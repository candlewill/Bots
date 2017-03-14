from corpus import Corpus
import jieba
import jieba.analyse

'''
本程序的目的是：自动将chatterbot训练数据转换为chatscript支持的对话脚本

chatterbot训练数据以json格式呈现，详见chinese目录

而chatscript对话脚本需要符合其本身的一些要求，详见其官网
'''


def sent_2_pattern(sent):
    topK = 5
    tf_idf_keywords = " ".join(
        sorted([x for x, _ in jieba.analyse.extract_tags(sent, withWeight=True, topK=topK)])).strip()

    text_rank_keywords = " ".join(
        sorted([x for x, _ in jieba.analyse.textrank(sent, withWeight=True, topK=topK)])).strip()

    seg_list = jieba.cut_for_search(sent)  # 搜索引擎模式
    search_keywords = " ".join(seg_list).strip()

    # text_rank 存在
    if text_rank_keywords is not None and text_rank_keywords:
        keywords = text_rank_keywords
    elif tf_idf_keywords is not None and tf_idf_keywords:
        keywords = tf_idf_keywords
    else:
        keywords = search_keywords

    pattern = "<< %s >>" % keywords

    '''
    # 其它可以考虑用来构造模式的方法：
    pattern = "[ ( << %s >> ) ( << %s >> ) ]" % (text_rank_keywords, search_keywords)
    pattern = "<< %s >>" % keywords
    pattern = "<< %s >>" % search_keywords
    '''

    return pattern


def generate_script(pattern_answer):
    topic_script = dict()  # key, value形式，key是模式，value是对应的回答，value类型是list
    for pa in pattern_answer:
        pattern, answer = pa
        if pattern in topic_script:
            # 一个pattern 多种回复
            topic_script[pattern].append(answer)
        else:
            # 建立一个新的回复
            topic_script[pattern] = [answer]

    topic_script = topic_script.items()
    topic_script = sorted(topic_script, key=lambda s: len(s[0]), reverse=True)
    return topic_script


def save_2_file(topic_script, topic_profile):  # 写入文件
    topic_name = topic_profile["topic_name"]
    mark = topic_profile["topic_mark"]

    first_line = "topic: ~%s %s ()\n" % (topic_name, mark)
    with open(topic_name + ".top", "w", encoding="utf-8", newline="") as f:
        f.write(first_line)
        for (pattern, answer) in topic_script:
            # 各种responder
            #  格式类似于：u: (can you calculate )
            #               1 plus 1 equals 2

            # 如果反馈只有一句话
            if len(answer) == 1:
                output = "\t%s\n" % answer[0]
            # 如果有多个
            else:
                output = "".join("\t[%s]\n" % ans for ans in answer)

            responder = "\nu: ( %s )\n%s" % (pattern, output)
            f.write(responder)


def main():
    data = Corpus()
    dotted_path = data.data_directory
    files = data.list_corpus_files(dotted_path)

    corpus = data.load_corpus(dotted_path)

    pattern_answer = []

    for topic in corpus:
        for dialog in topic:
            if len(dialog) == 2:
                pattern = sent_2_pattern(dialog[0])
                pattern_answer.append([pattern, dialog[1]])

    topic_script = generate_script(pattern_answer)
    save_2_file(topic_script, topic_profile)


if __name__ == '__main__':
    topic_profile = {"topic_name": "keywordless",  # 话题名称
                     "topic_mark": "nostay keep repeat"
                     }
    main()
