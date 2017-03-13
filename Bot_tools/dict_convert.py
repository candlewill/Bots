import codecs
import sys

'''
本程序目的是将Jieba词典转换为ChatScript所要求的格式

Jieba词典格式如下：
大后年 3 t
大后方 49 n
大吏 118 n
大吐苦水 3 i
大君 9 n
大含细入 3 n
大吵 3 v
大吵一架 3 n
大吵大闹 30 l
大吹大打 3 i
大吹大擂 30 i
大吹法螺 5 n
大吼大叫 3 n
大员 154 n

ChatScript所要求的格式如下：
order_Strigiformes ( meanings=1 glosses=1 NOUN_ABSTRACT NOUN NOUN_PROPER_SINGULAR )
    Strigiformes~1nz (^animal_order~1) owls
 ozone_sickness ( meanings=1 glosses=1 NOUN_ABSTRACT NOUN NOUN_SINGULAR NOUN_NODETERMINER )
    ozone_sickness~1nz (^sickness~1) illness that can occur to persons exposed to ozone in high-altitude aircraft
 onion ( meanings=3 glosses=3 NOUN NOUN_SINGULAR COMMON4 NOUN_NODETERMINER KINDERGARTEN posdefault:NOUN )
    onion~1nz (^vegetable~1) an aromatic flavorful vegetable
    Allium_cepa~1nz (^alliaceous_plant~1) bulbous plant having hollow leaves cultivated worldwide for its rounded edible bulb
    onion~3nz (^bulb~5) the bulb of an onion plant
 oilbird ( meanings=1 NOUN NOUN_SINGULAR COMMON1 posdefault:NOUN )
    guacharo~1n
 ocellated ( meanings=1 glosses=1 ADJECTIVE ADJECTIVE_NORMAL COMMON1 posdefault:ADJECTIVE )
    ocellated~1az having ocelli
'''

def main():
    input_file = "jieba.dict.utf8"
    input_char="char.txt"
    output_file = "output.txt"

    data = codecs.open(input_file, encoding="utf-8").readlines()
    chars = codecs.open(input_char, encoding="utf-8").readline()
    chars = set(list(chars))

    words=set()
    for line in data:
        if line.strip():
            word, _, _ = line.strip().split(" ")
            words.add(word)

    with codecs.open(output_file, encoding="utf-8", mode="w") as f:
        for word in words.union(chars):
            f.write(" " + word + " ( NOUN ) \n")

if __name__ == '__main__':
    main()
