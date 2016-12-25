#!/usr/bin/env python3
# print sorted killcount for monsters in a savefile

import sys
import json
import operator

if len(sys.argv) != 2:
    print('Use: ' + sys.argv[0] + ' <savefile.sav>')
    sys.exit(1)
savefile = str(sys.argv[1])

with open(savefile, 'r') as fd:
    next(fd) # skip version header
    save = json.load(fd)

kills = sorted(save['kills'].items(), key = operator.itemgetter(1), reverse = True)
killstotal = sum(killcount for _, killcount in kills)

for monster, killcount in kills:
    percentage = 100 * float(killcount) / float(killstotal)
    print(monster, killcount, "%.2f" % percentage)
