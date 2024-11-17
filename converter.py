import argparse, sc2reader
from sc2reader.events.game import *
from sc2reader.events.message import *
from sc2reader.events.tracker import *

def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('DirectoryName', type=str, help="the directory to parse")
    parser.add_argument('Window', type=int, help="15, 30, or 45")

    args = parser.parse_args()
    
    dictionary = {}
    id = 1
    maxSeconds = 600
    windowSeconds = args.Window
    
    for filename in sc2reader.utils.get_files(args.DirectoryName):
        replay = sc2reader.load_replay(filename, load_level=4, debug=True)
        players = [t.players for t in replay.teams]
        players = [y for x in players for y in x]
        matchup = [p.play_race[0:1] for p in players]
        matchup.sort()
        matchup="".join(matchup)
        replay_info = [matchup, replay.release_string, replay.type, replay.map_name, replay.start_time]
        
        if len(players) != 2: continue
        sec = 0
        nextItemset = False
        for event in replay.events:
            if (event.second > maxSeconds): break
            if (isinstance(event, TargetPointCommandEvent) and event.has_ability and event.ability_name.startswith("Build")):
                action = event.ability_name[5:]
                if event.player.result[:1] == "W": 
                    action = "+" + action
                else: 
                    action = "-" + action
                if (action not in dictionary):
                    dictionary[action] = id
                    id += 1
                print(dictionary[action], end = " ")
                nextItemset = False
                if (event.second - sec > windowSeconds):
                    sec = event.second
                    print("-1", end=" ")
                    nextItemset = True
        if (not nextItemset): 
            print ("-1", end=" ")
        print("\t".join([str(s) for s in ["-2"]+replay_info]))
        
    f = open("dictionary.txt", "w")
    for a in dictionary:
        f.write("{1}\t{0}\n".format(dictionary[a], a))
     
main()