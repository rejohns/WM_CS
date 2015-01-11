#!/usr/bin/python
import psycopg2
import getpass
import sys
from decimal import *
import random

def player_lookup(): #check
    print

    stat_type = raw_input("Select a type (hit/ pitch/ field): ").lower()
    if stat_type != "hit" and stat_type != "pitch" and stat_type != "field" and\
       stat_type != "q":
        print "Invalid choice.  Select again."
        player_lookup()
    elif stat_type == "q":
        main()
    player = raw_input("Select a player: ")
    if player == "q":
        main()
    year = raw_input("Select a year: ")
    if year == "q":
        main()
    else:
        player_lookup_sql(stat_type, player, year)
    
def team_lookup():
    print
    stat_type = raw_input("Select a type (hit/ pitch/ field): ").lower()
    if stat_type != "hit" and stat_type != "pitch" and stat_type != "field" and\
       stat_type != "q":
        print "Invalid choice.  Select again."
        player_lookup()
    elif stat_type == "q":
        main()     
    team = raw_input("Select a team: ")
    if team.lower() not in team_full and team != "q":
        print "Invalid choice. Select again."
        team_lookup()
    elif team == "q":
        main()
    year = raw_input("Select a year: ").lower()
    if year == "q":
        main()
    team_lookup_sql(stat_type, team, year)
    
def player_comparison():
    print 
    compare_list = []
    stat_type = raw_input("Select a type (hit/ pitch/ field): ").lower()
    if stat_type != "hit" and stat_type != "pitch" and stat_type != "field" and\
       stat_type != "q":
        print "Invalid choice.  Select again."
        player_lookup()
    elif stat_type == "q":
        main()    
    while True:
        print "Type 'done' to finish selection, 'q' to quit"
        player = raw_input("Choose a player: ")
        if player == 'q':
            main()
        elif player == 'done':
            break
        year = raw_input("Choose a year: ")
        if year == 'q':
            main()
        elif year == 'done':
            break        
        print 
        pyList = [player, year]
        compare_list.append(pyList)
    player_comparison_sql(stat_type, compare_list)
        
    
def team_comparison():
    print 
    compare_list = []
    stat_type = raw_input("Select a type (hit/ pitch/ field): ").lower()
    if stat_type != "hit" and stat_type != "pitch" and stat_type != "field" and\
       stat_type != "q":
        print "Invalid choice.  Select again."
        team_lookup()
    elif stat_type == "q":
        main()    
    while True:
        print "Type 'done' to finish selection, 'q' to quit"
        team = raw_input("Choose a team: ")
        if team == 'q':
            main()
        elif team == 'done':
            break
        if team.lower() not in team_full and team != "q":
            print "Invalid choice. Select again."
            team_comparison()
        year = raw_input("Choose a year: ")
        if year == 'q':
            main()
        elif year == 'done':
            break        
        print 
        pyList = [team, year]
        compare_list.append(pyList)
    team_comparison_sql(stat_type, compare_list)
    
def league_leaders(): #check
    all_years = False
    stat_type = raw_input("Select a type (hit/ pitch/ field): ").lower()
    if stat_type != "hit" and stat_type != "pitch" and stat_type != "field" and\
       stat_type != "q":
        print "Invalid choice.  Select again."
        league_leaders()
    elif stat_type == 'q':
        main()
    year = raw_input("Choose a year (select 'all' for 1998-2012): ")
    if year == 'q':
        main()
    elif year == 'all':
        all_years = True
    stat = raw_input('Select a statistic: ')
    if stat == 'q':
        main()
    elif stat_type == 'hit' and stat not in hit_stats:
        print "Invalid choice.  Select again."
        league_leaders()
    elif stat_type == 'pitch' and stat not in pitch_stats:
        print "Invalid choice.  Select again."
        league_leaders()
    elif stat_type == 'field' and stat not in field_stats:
        print "Invalid choice.  Select again."
        league_leaders()
    league_leaders_sql(stat_type, year, stat)
    
def head_to_head():
    team1 = raw_input("Select a team: ")
    if team1.lower() not in team_full and team1.lower() != "q":
        print "Invalid choice. Select again."
        head_to_head
    elif team1 == "q":
        main()
    year1 = raw_input("Select a year: ")   
    if year1 == "q":
        main()    
    team2 = raw_input("Select another team: ")
    if team2.lower() not in team_full and team2.lower() != "q":
        print "Invalid choice. Select again."
        head_to_head()
    elif team2 == "q":
        main()
    year2 = raw_input("Select a year: ")
    if year2 == "q":
        main()
    runScore_query1 = 'select r from team_hit where tname = %s' + ' and' \
        + ' tyear =' + year1
    
    runScore_query2 = 'select r from team_hit where tname = %s' + ' and' \
        + ' tyear =' + year2 
    
    runAllow_query1 = 'select r from team_pitch where tname = %s' + ' and' \
        + ' tyear =' + year1 
    runAllow_query2 = 'select r from team_pitch where tname = %s' + ' and' \
        + ' tyear =' + year2
    
    t1_winsQ = 'select w, l, wl from team_pitch where tname = %s' + ' and' \
        + ' tyear =' + year1
    t2_winsQ = 'select w, l, wl from team_pitch where tname = %s' + ' and' \
            + ' tyear =' + year2    

    teamName1 = []
    teamName1.append(team1)
    teamName2 = []
    teamName2.append(team2)

    cur.execute(t1_winsQ, teamName1)
    tuples = []
    for Tuple in cur:
        tuples.append(Tuple)
    if tuples == []:
        print "Error. Try again."
        head_to_head()
    else:       
        t1_wins = tuples[0][0] 
        t1_losses = tuples[0][1] 
        t1_wl = float(tuples[0][2])     
    
    cur.execute(t2_winsQ, teamName2)
    tuples = []
    for Tuple in cur:
        tuples.append(Tuple) 
    if tuples == []:
        print "Error. Try again."
        head_to_head()    
    else:    
        t2_wins = tuples[0][0] 
        t2_losses = tuples[0][1] 
        t2_wl = float(tuples[0][2]) 
    
        
    cur.execute(runScore_query1, teamName1)
    
    tuples = []
    for Tuple in cur:
        tuples.append(Tuple)   
        
    t1_runScore = tuples[0][0]   
    
    cur.execute(runScore_query2, teamName2)
    tuples = []
    for Tuple in cur:
        tuples.append(Tuple)   
            
    t2_runScore = tuples[0][0]   
    
    cur.execute(runAllow_query1, teamName1)
    tuples = []
    for Tuple in cur:
        tuples.append(Tuple)   
            
    t1_runAllow = tuples[0][0]  
    
    cur.execute(runAllow_query2, teamName2)
    tuples = []
    for Tuple in cur:
        tuples.append(Tuple)   
            
    t2_runAllow = tuples[0][0]     
    
    t1_pythagPC = (t1_runScore**1.83)/((t1_runScore**1.83) + (t1_runAllow**1.83))
    t2_pythagPC = (t2_runScore**1.83)/((t2_runScore**1.83) + (t2_runAllow**1.83))
    
    t1_pythagW = int(t1_pythagPC*162)
    t2_pythagW = int(t2_pythagPC*162)
    t1_pythagL = 162-t1_pythagW
    t2_pythagL = 162-t2_pythagW
    
    t1luck = t1_pythagPC - t1_wl
    t2luck = t2_pythagPC - t2_wl
    
    print
    print "In regular season play:"
    print team1 + ' (' + year1 + '): ' + 'wins: ' + str(t1_wins) + ' losses: ' + str(t1_losses)
    print team2 + ' (' + year2 + '): ' + 'wins: ' + str(t2_wins) + ' losses: ' + str(t2_losses)
    
    print
    
    print "Pythagorean expected winning percentage:"
    print team1 + ' ' + year1 + ': ' + str(round(t1_pythagPC,3))
    print 'Record: ' + str(t1_pythagW) + '-' + str(t1_pythagL)
    print team2 + ' ' + year2 + ': ' + str(round(t2_pythagPC,3))
    print 'Record: ' + str(t2_pythagW) + '-' + str(t2_pythagL)
    
    print
    print "Simulated 7-game playoff series:"
    print
    
    game = 1
    count1 = 0
    count2 = 0
    gameResult = []
    while game <= 7 and count1 < 4 and count2 < 4:
        rand1 = random.uniform(0,.6)
        rand2 = random.uniform(0,.6)
        t1Score = t1_pythagPC + t1luck + rand1
        t2Score = t2_pythagPC + t2luck + rand2
        
        if t1Score > t2Score:
            count1 += 1
            gameResult.append("Game " + str(game) + ": " + team1 + " " + str(year1))
        else:
            count2 += 1
            gameResult.append("Game " + str(game) + ": " + team2 + ' ' + str(year2))
        game += 1
    
    for element in gameResult:
        print element
    if count1 > count2:
        winner = team1
        yearW = year1
    else:
        winner = team2
        yearW = year2
    print
    print "Series winner: " + winner + ' ' + str(yearW)
    print
    quit_input = raw_input("Enter anything to go back to the main menu: ")
    main()
    

def custom_stat_player():
    print
    opList = ["(", ")", "/", "+", "-", "*", "**"]
    operands = []
    tempList = [] 
    player = raw_input("Select a player: ")
    if player.lower() == 'q':
        main()
    year = raw_input("Select a year: ")
    if year.lower() == "q":
        main()
    statName = raw_input("Enter name of your statistic: ")
    print "Please surround all stats with '()'" 
    formula = raw_input("Enter a formula: ")
    if formula.lower() == 'q':
        main()
    
    opBool = True
    strBool = True
    count = 0
    
    for char in formula:
    
        if char in opList:
            
            if count > 0 and opBool == False:
                operand = ''.join(tempList)
                operands.append(operand)
                tempList = []
            opBool = True
            
        elif char not in opList:
            tempList.append(char)
            opBool = False
     
        count += 1    
    count = 0
    for item in operands:
        if item not in hit_stats and item not in pitch_stats and item not in field_stats:
            print '"' + item + '" ' + "is not a valid statistic."
            custom_stat()
    
        if item in hit_stats:
            query = 'select ' + item + ' from player_hit where pName = %s' \
                + ' and pYear = ' + year
        elif item in pitch_stats:
            query = 'select ' + item + ' from player_pitch where pName = %s' \
                           + ' and pYear = ' + year
        elif item in field_stats:
            query = 'select ' + item + ' from player_field where pName = %s' \
                           + ' and pYear = ' + year            
        lookup = [player]
        cur.execute(query, lookup)
        tuples = []
        for Tuple in cur:
            tuples.append(Tuple)        
        stat = tuples[0][0]
        operands[count] = float(stat)
        
        count += 1  
    formulaList = []
    for char in formula:
        if char in opList:
            formulaList.append(char)
    
    count = 0
    opIndex = 0
    for char in formulaList:
        if char == ")" and lastChar == "(":
            formulaList.insert(count, str(operands[opIndex]))
            opIndex += 1
                    
        lastChar = char
        count += 1    

    formulaString = formula
    formula = ''.join(formulaList)
    result = eval(formula)
    
    print
    print player + "'s " + year + " " + statName + " value: " + str(round(result,3))
    upload = raw_input("Would you like to upload your custom statistic? (y/n): ")
    if upload.lower() == "y":
        customs = [player, int(year), statName, formulaString, round(float(result),3)]
        cur.execute("insert into custom_player values (%s, %s, %s, %s, %s)",customs)
        connection.commit()
    else:
        main()
        
def custom_stat_team():
    print
    opList = ["(", ")", "/", "+", "-", "*", "**"]
    operands = []
    tempList = [] 
    team = raw_input("Select a team: ")
    if team.lower() not in team_full and team.lower() != 'q':
        print "Invalid choice. Select again."
        custom_stat_team()
    elif team.lower() == 'q':
        main()
    year = raw_input("Select a year: ")
    if year == 'q':
        main()
    statName = raw_input("Enter name of your statistic: ")
    print "Please surround all stats with '()'" 
    formula = raw_input("Enter a formula: ")
    if formula.lower() == 'q':
        main()    
    
    opBool = True
    strBool = True
    count = 0
    
    for char in formula:
    
        if char in opList:
            
            if count > 0 and opBool == False:
                operand = ''.join(tempList)
                operands.append(operand)
                tempList = []
            opBool = True
            
        elif char not in opList:
            tempList.append(char)
            opBool = False
     
        count += 1    
    count = 0
    for item in operands:
        if item not in hit_stats and item not in pitch_stats and item not in field_stats:
            print '"' + item + '" ' + "is not a valid statistic."
            custom_stat()
    
        if item in hit_stats:
            query = 'select ' + item + ' from team_hit where tName = %s' \
                + ' and tYear = ' + year
        elif item in pitch_stats:
            query = 'select ' + item + ' from team_pitch where tName = %s' \
                           + ' and tYear = ' + year
        elif item in field_stats:
            query = 'select ' + item + ' from team_field where tName = %s' \
                           + ' and yYear = ' + year            
        lookup = [team]
        cur.execute(query, lookup)
        tuples = []
        for Tuple in cur:
            tuples.append(Tuple)        
        stat = tuples[0][0]
        operands[count] = float(stat)
        
        count += 1  
    formulaList = []
    for char in formula:
        if char in opList:
            formulaList.append(char)
    
    count = 0
    opIndex = 0
    for char in formulaList:
        if char == ")" and lastChar == "(":
            formulaList.insert(count, str(operands[opIndex]))
            opIndex += 1
                    
        lastChar = char
        count += 1    

    formulaString = formula
    formula = ''.join(formulaList)
    result = eval(formula)
    
    print
    print "The " + team + "'s " + year + " " + statName + " value: " + str(round(result,3))
    upload = raw_input("Would you like to upload your custom statistic? (y/n): ")
    if upload.lower() == "y":
        customs = [team, int(year), statName, formulaString, round(float(result),3)]
        cur.execute("insert into custom_team values (%s, %s, %s, %s, %s)",customs)
        connection.commit()
    else:
        main()
    
def view_custom_stats():
    choice = raw_input("Would you like to view custom player stats or custom team stats? (p/t): ")
    print
    if choice.lower() == "p":
        query = "select * from custom_player order by statname"
        cur.execute(query)
        Tuples = []
        for item in cur:
            Tuples.append(item)
        print "Name           Year  StatName  Formula     Value  "
        for item in Tuples:
            print item
    elif choice.lower() == "t":
        query = "select * from custom_team order by statname"
        cur.execute(query)
        Tuples = []
        for item in cur:
            Tuples.append(item)
        print "Team             Year  StatName  Formula          Value  "
        for item in Tuples:
            print item     
    elif choice.lower == "q":
        main()
    else:
        print ("Invalid input. Try again.")
        view_custom_stats()

#SQL HELPER METHODS

def player_lookup_sql(stat_type, player, year):
    if stat_type == "hit":
        relation = "player_hit"
        p_stats = hit_stats
    elif stat_type == "pitch":
        relation = 'player_pitch'
        p_stats = pitch_stats
    else:
        relation = 'player_field'
        p_stats = field_stats
    query = 'select * from ' + relation + ' where pName = %s' + ' and pYear = '\
        + year
    lookup = [player]
    cur.execute(query, lookup)
    tuples = []
    for Tuple in cur:
        tuples.append(Tuple)
    if tuples == []:
        print
        print "You have selected an incorrect player or year. Choose again."
        player_lookup()
        
    print
    print "%-10s" % ("Team"), tuples[0][0]
    print "%-10s" % ("Year"), tuples[0][1]
    if stat_type != "field":
        print "%-10s" % ("Position"), tuples[0][2]
        print "%-10s" % ("Name"), tuples[0][3]
        print "%-10s" % ("Age"), tuples[0][4]
    else:
        print "%-10s" % ("Name"), tuples[0][2]
        print "%-10s" % ("Age"), tuples[0][3]        
    
    if stat_type == "field":
        count = 4
    else:    
        count = 5
    for stat in p_stats:
        print "%-10s" % (stat), tuples[0][count]
        count += 1
    print
    quit_input = raw_input("Enter anything to go back to main menu: ")
    main()
    
def team_lookup_sql(stat_type, team, year):
    if stat_type == "hit":
        relation = "team_hit"
        statList = team_hit_stats
    elif stat_type == "pitch":
        relation = 'team_pitch'
        statList = team_pitch_stats
    else:
        relation = 'team_fielding'
        statList = team_field_stats
    query = 'select * from ' + relation + ' where tName = %s' + ' and tYear = '\
        + year
    lookup = [team]
    cur.execute(query, lookup)
    tuples = []
    for Tuple in cur:
        tuples.append(Tuple)
    if tuples == []:
        print
        print "You have selected an incorrect team or year. Choose again."
        team_lookup()
      
      
    print
    count = 0
    for tStat in statList:
        print "%-8s" % (tStat), tuples[0][count]
        count += 1
    print
    
    roster = raw_input("Would you like to view the roster from " + year + "? (y/n): ")
    print
    if roster.lower() == "y":
        roster = []
        query2 = "select name from player where team = %s" + " and year = " + year
        cur.execute(query2, lookup)
        for Tuple in cur:
            roster.append(Tuple)
        print "Roster:"
        print
        for item in roster:
            print item[0]
    else:
        print
        quit_input = raw_input("Enter anything to go back to main menu: ")
        main()   
    print
    quit_input = raw_input("Enter anything to go back to main menu: ")
    main()    
    
def player_comparison_sql(stat_type, compare_list):
    if compare_list == []:
        print "You have not selected any players to compare. Please try again."
        player_comparison()
    if stat_type == "hit":
        relation = "player_hit"
        pStat = hit_stats
    elif stat_type == "pitch":
        relation = 'player_pitch'
        pStat = pitch_stats
    else:
        relation = 'player_field'
        pStat = field_stats
    tuples = []
    for element in compare_list:
        query = 'select * from ' + relation + ' where pName = %s' + ' and pYear = %s'
        lookup = element
        cur.execute(query, lookup)
        for Tuple in cur:
            tuples.append(Tuple)
    if tuples == []:
        print
        print "None of the players you chose played in your selected year."
        print "Please try again"
        player_comparison()
    
    print
    print "%-21s"%("Team"), " Year", " Pos ", "Name  ", "   Age",
    statString = ""
    
   
    for stat in pStat:
        statString += '  '
        statString += str(stat) 
        
    print statString
    
    
    for Tuple in tuples:
        playerList = []
        for item in Tuple:
            playerList.append(item)
            
        playerString = ""
        count = 0
        for element in playerList:
            
            playerString += str(element)
            if count == 0:
                playerString += (21-len(element))*" "
            else:
                playerString += " "
            count += 1
            
        print playerString
    
    print 
    quit_input = raw_input("Enter anything to go back to main menu: ")
    
def team_comparison_sql(stat_type, compare_list):
    if compare_list == []:
        print "You have not selected any teams to compare. Please try again."
        team_comparison()
    if stat_type == "hit":
        relation = "team_hit"
        tStat = team_hit_stats
    elif stat_type == "pitch":
        relation = 'team_pitch'
        tStat = team_pitch_stats
    else:
        relation = 'team_fielding'
        tStat = team_field_stats
    tuples = []
    for element in compare_list:
        query = 'select * from ' + relation + ' where tName = %s' + ' and tYear = %s'
        lookup = element
        cur.execute(query, lookup)
        for Tuple in cur:
            tuples.append(Tuple)
    if tuples == []:
        print
        print "There is no data for any of your selected teams in your selected years."
        print "Please try again"
        team_comparison()
    print
    for item in tStat:
        print item + "   ",
    print
    
    for Tuple in tuples:
        statList = []
        statString = ""
        for element in Tuple:
            statList.append(element)
        for item in statList:
            statString += str(item) + "  "
        print statString
    print
    quit_input = raw_input("Enter anything to go back to main menu: ")
    main()
    
def league_leaders_sql(stat_type, year, stat):
    if stat_type == "hit":
            relation = "player_hit"
            qual_type = 'ab'
            qual_quant = 300
    elif stat_type == "pitch":
        relation = 'player_pitch'
        qual_type = 'ip'
        qual_quant = 60       
    else:
        relation = 'player_field'
        qual_type = 'inn'
        qual_quant = 300        
    AscDesc = raw_input("Would you like the highest first or lowest first? (h/l): ")
    print
    if AscDesc.lower() == "h":
        if year.lower() == 'all':
            query = 'select pTeam, pYear, pName, pos, ' + stat + ' from ' + relation + ' where ' + stat + ' > 0 and ' + qual_type + ' > ' + str(qual_quant) + ' order by ' + stat + ' desc limit 25'
        else:
            query = 'select pTeam, pYear, pName, pos, ' + stat + ' from ' + relation + ' where pYear=' +year+ ' and ' + stat + ' > 0 and ' + qual_type + ' > ' + str(qual_quant) + '  order by ' +stat+ ' desc limit 25'
    elif AscDesc.lower() == 'l':
        if year.lower() == 'all':
            query = 'select pTeam, pYear, pName, pos, ' + stat + ' from ' + relation + ' where ' + stat + ' > 0 and ' + qual_type + ' > ' + str(qual_quant) + ' order by ' + stat + ' limit 25'
        else:
            query = 'select pTeam, pYear, pName, pos, ' + stat + ' from ' + relation + ' where pYear=' +year+ ' and ' + stat + ' > 0 and ' + qual_type + ' > ' + str(qual_quant) + '  order by ' +stat+ ' limit 25'
    elif AscDesc.lower() == 'q':
        main()
    else:
        print "Invalid input. Please try again."
        league_leaders_sql(stat_type, year, stat)
    cur.execute(query)
    print "Team                Year       Name           Pos  ", stat
    tuples= []
    for Tuple in cur:
        tuples.append(Tuple)
        print Tuple
    if tuples == []:
        print "You have made an error. Please try again."
        league_leaders()
    print
    quit_input= raw_input("Enter anything to go back to main menu: ")

#MAIN METHOD
    
def main():

    user_input = "h" 
    while user_input != "q":
        print
        print "Choose a function"
        print "a) Player lookup by name & year"
        print "b) Team lookup by name & year"
        print "c) Player comparison"
        print "d) Team comparison"
        print "e) League leaders by year"
        print 'f) Head-to-head team comparison'
        print 'g) Build your own statistic for players'
        print 'h) Build your own statistic for teams'
        print 'i) View your custom statistics'
        print
        
        user_input = raw_input("Enter a choice: ").lower()
        
        if user_input == "a":
            player_lookup()
        
        elif user_input == "b":
            team_lookup()
            
        elif user_input == "c":
            player_comparison()            
        
        elif user_input == "d":
            team_comparison()  
            
        elif user_input == "e":
            league_leaders()
            
        elif user_input == 'f':
            head_to_head()
            
        elif user_input == 'g':
            custom_stat_player()
            
        elif user_input == 'h':
            custom_stat_team()
            
        elif user_input == 'i':
            view_custom_stats()
        
        elif user_input == "q":
            connection.close()
            sys.exit(None)
            
        else:
            print
            print "Invalid choice. Choose again."
            print 

team_abbrev = ('nyy', 'bal', 'tbr', 'tor', 'bos', 'det', 'cws', 'cle', 'kcr',\
               'min', 'oak', 'tex', 'laa', 'sea', 'wsn', 'atl', 'phi', 'nym',\
               'mia', 'cin', 'stl', 'mil', 'pit', 'chc', 'hou', 'sfg', 'lad',\
               'ari', 'sdp', 'col')

team_full = ('new york yankees', 'baltimore orioles', 'tampa bay rays',\
             'toronto blue jays', 'boston red sox', 'detroit tigers',\
             'chicago white sox', 'cleveland indians', 'kansas city royals',\
             'minnesota twins', 'oakland athletics', 'texas rangers',\
             'los angeles angels', 'seattle mariners', 'washington nationals',\
             'atlanta braves', 'philadelphia phillies', 'new york mets',\
             'miami marlins', 'cincinnati reds', 'st. louis cardinals',\
             'milwaukee brewers', 'pittsburgh pirates', 'chicago cubs',\
             'houston astros', 'san francisco giants', 'los angeles dodgers',\
             'arizona diamondbacks', 'san diego padres', 'colorado rockies')

team_name = ('yankees', 'orioles', 'rays', 'blue jays', 'red sox', 'tigers',\
             'white sox', 'indians', 'royals', 'twins', 'athletics', 'rangers',\
             'angels', 'mariners', 'nationals', 'braves', 'phillies', 'mets',\
             'marlins', 'reds', 'cardinals', 'brewers', 'pirates', 'cubs',\
             'astros', 'giants', 'dodgers', 'diamondbacks', 'padres', 'rockies')

hit_stats = ('g', 'pa', 'ab', 'r', 'h' , 'doubles', 'triples', 'hr', 'rbi',\
             'sb', 'cs', 'bb', 'so', 'ba', 'obp', 'slg', 'ops', 'opsplus',\
             'tb', 'gdp', 'hbp', 'sh', 'sf', 'ibb')

pitch_stats = ('wins', 'losses', 'wl', 'era', 'g', 'gs', 'gf', 'cg', 'sho',\
               'sv', 'ip', 'h', 'r', 'er', 'hr', 'bb', 'ibb', 'so', 'hbp',\
               'bk', 'wp', 'bf' 'eraplus', 'whip', 'h9inn', 'hr9inn', 'bb9inn',\
               'so9inn', 'sobb')
field_stats = ('g', 'gs', 'cg', 'inn', 'ch', 'po', 'a', 'e', 'dp', 'fld','rtot',
               'rtotyr', 'rdrs', 'rdrsyr', 'rf9', 'rfg', 'pb', 'wp', 'sb','cs',\
               'csp', 'lgcs', 'pickoff', 'possum')
team_pitch_stats = ('tname','tyear', 'numpitch','aveage','rg','w','l','wl','era','g'  ,'gs' ,     'gf' ,'cg' ,'tmsho','cgsho' ,'sv'  ,'ip','h' ,'r' ,'er' ,'hr' ,'bb' ,'ibb','so' ,'hbp','bk' ,     'wp' ,'bf' ,'eraplus','whip','h9'  ,'hr9' ,'bb9' ,'so9','sobb')  

team_hit_stats = ('tname', 'tyear', 'numbat', 'average', 'rg', 'g', 'pa', 'ab','r','h','doubles','triples','hr','rbi','sb','cs','bb','so','ba','obp','slg','ops','opsplus','tb','gdp','hbp','sh','sf','ibb')

team_field_stats = ('tname', 'tyear','numfield','rg','defeff','g','gs','cg','inn' \
                    ,'ch','po','a','e','dp','fld','rtot','rtotyr','rdrs','rdrsyr')

try:
    connection = psycopg2.connect(database = "bskayton_baseball",\
                                  user = "bskayton", password = 'bskayton')
        
except StandardError, e:
    print str(e)
    exit

cur= connection.cursor()

main()