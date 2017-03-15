import codecs
import json
import re

'''
本程序的目的是将小黄鸡数据转为chatterbot格式

小黄鸡数据格式：
E
M 那你有男朋友么
M 没有呢，我只要主人一个人疼爱我嘛。。
E
M 那你在哪
M 我无聊
E
M 你妈是谁
M 我妈妈是世界上最温柔的主人你啊
E
M 去你大爷的
M 我去，滚你大爷的！
E
M 你在骂我一句
M 你果然够虚荣~

chatterbot格式
{
    "greetings": [
        [
            "怎么了?",
            "没什么,你呢?"
        ],
        [
            "怎么了?",
            "没啥."
        ],
        [
            "怎么了?",
            "没事谢谢，你呢?"
        ]
    ]
}
'''

reg = "[A-Za-z0-9\[\`\~\!\@\#\$\^\&\*\(\)\=\|\{\}\'\:\;\'\,\[\]\.\<\>\/\?\~\！\@\#\\\&\*\%\】\【\͡\…\★\—\°\…\、\͡\̸\。\”\〒\_\“\•\⊙\？\╯\□\╰̨]+"


def simple_preprocess(string):
    # from M 傻瓜   现在在你身边看着的人就是你的她    你要对她好   宠她   爱她   知道了吗？
    # to 傻瓜 现在在你身边看着的人就是你的她 你要对她好 宠她 爱她 知道了吗？
    processed_text = " ".join(string.strip().split()[1:])
    processed_text = re.sub("[\~\@\^\[\]\(\)]+", " ", processed_text)
    return processed_text


def complex_preprocess(data):
    global reg
    processed_data = []
    for (question, answer) in data:
        question = re.sub(reg, " ", question)
        question = re.sub(" +", " ", question)
        if len(question) >= 2:
            processed_data.append((question, answer))
    return processed_data


def load_data(filename):
    data = []
    with codecs.open(filename, encoding="utf-8") as f:
        volley = []
        i = 0
        for line in f:
            i += 1
            if line.startswith("M "):

                if len(volley) == 2:
                    question = simple_preprocess(volley[0])
                    answer = simple_preprocess(volley[1])
                    data.append((question, answer))
                    if debug:
                        print((question, answer))
                    volley = []

                volley.append(line)

                if debug:
                    if (i > 20000): break
    return data


def convert_2_json(data):
    # 转成符合chatterbot训练数据格式的json文本
    data = complex_preprocess(data)
    out = dict()
    out["xiaohuangji"] = data
    return out


def save(data, filename):
    # 保存json数据
    with open(filename, 'w', encoding="utf-8", newline="") as fp:
        json.dump(data, fp, sort_keys=True, indent=4, ensure_ascii=False)


def main():
    input_file = "./raw/xiaohuangji.txt"
    output_file = "./data/xiaohuangji.json"

    data = load_data(input_file)
    json_data = convert_2_json(data)
    save(json_data, output_file)


if __name__ == '__main__':
    debug = True
    main()
