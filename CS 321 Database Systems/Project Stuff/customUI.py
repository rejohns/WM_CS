#!/usr/bin/python
import psycopg2
import getpass
import sys

def player_lookup():
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
    player_lookup_sql(stat_type, player, year)
    
def team_lookup():
    print
    team = raw_input("Select a team: ").lower()
    if team not in team_abbrev and team not in team_full and\
       team not in team_name and team != "q":
        print "Invalid choice. Select again."
        team_lookup()
    elif team == "q":
        main()
    year = raw_input("Select a year: ").lower()
    if year == "q":
        main()
    
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
        player = raw_input("Choose a player: ").lower()
        if player == 'q':
            main()
        elif player == 'done':
            break
        year = raw_input("Choose a year: ").lower()
        if year == 'q':
            main()
        elif year == 'done':
            break        
        print 
        pyList = [player, year]
        compare_list.append(pyList)
        
    
def team_comparison():
    print 
    compare_list = []  
    while True:
        print "Type 'done' to finish selection, 'q' to quit"
        team = raw_input("Choose a team: ").lower() 
        if team not in team_abbrev and team not in team_full and\
           team not in team_name and team != "q":
            print "Invalid choice. Select again."
            team_comparison()        
        if team == 'q':
            main()
        elif team == 'done':
            break
        year = raw_input("Choose a year: ").lower()
        if year == 'q':
            main()
        elif year == 'done':
            break        
        print 
        pyList = [team, year]
        compare_list.append(pyList)
    
def league_leaders():
    all_years = False
    stat_type = raw_input("Select a type (hit/ pitch/ field): ").lower()
    if stat_type != "hit" and stat_type != "pitch" and stat_type != "field" and\
       stat_type != "q":
        print "Invalid choice.  Select again."
        league_leaders()
    elif stat_type == 'q':
        main()
    year = raw_input("Choose a year (select 'all' for 1998-2012)")
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
    
def head_to_head():
    team1 = raw_input("Select a team: ").lower()
    if team1 not in team_abbrev and team1 not in team_full and\
       team1 not in team_name and team1 != "q":
        print "Invalid choice. Select again."
        head_to_head
    elif team1 == "q":
        main()
    year1 = raw_input("Select a year: ")
    if ((year1 <= 1997) or (year1 >= 2013)):
        print "Invalid choice. Select again."
        head_to_head()    
    elif year1 == "q":
        main()    
    team2 = raw_input("Select a team: ").lower()
    if team2 not in team_abbrev and team2 not in team_full and\
       team2 not in team_name and team2 != "q":
        print "Invalid choice. Select again."
        head_to_head()
    elif team2 == "q":
        main()
    year2 = raw_input("Select a year: ")
    if ((year2 <= 1997) or (year2 >= 2013)):
        print "Invalid choice. Select again."
        head_to_head()    
    elif year2 == "q":
        main()
        
    

def custom_stat():
    print
    opList = ["(", ")", "/", "+", "-", "*", "^"]
    operands = []
    tempList = [] 
    statType = raw_input("Would you like to build a statistic for a player or a team(p/t)?").lower()
    if statType == "p":
        player = raw_input("Select a player: ")
        year = raw_input("Select a year: ")
        if ((year2 <= 1997) or (year2 >= 2013)):
            print "Invalid choice. Select again." 
            custom_stat()
    elif statType == "t":
        team = raw_input("Select a team: ")                # check for invalid? lowercase in database?
        year = raw_input("Select a year: ")
        if ((year2 <= 1997) or (year2 >= 2013)):
            print "Invalid choice. Select again." 
            custom_stat()     
            
    print "Please surround all stats with '()'"    
    formula = raw_input("Enter a formula: ")
    
    
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
    
    # Iterate through stats, set up queries                                 # have to still add functionality for teams
    count = 0
    for item in operands:
        if item not in hit_stats or item not in pitch_stats or item not in field_stats:
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
        #...
        # stat = query stuff
        operands[count] = stat
        
        count += 1
        
     # Set up equation, operators order
    formulaList = []
    for char in stringFormula:
        if char in opList:
            formulaList.append(char)
    
     # Insert stat values into equation
    count = 0
    opIndex = 0
    for char in formulaList:
        if char == ")" and lastChar == "(":
            formulaList.insert(count, str(operands[opIndex]))
            opIndex += 1
                    
        lastChar = char
        count += 1    

    formula = ''.join(formulaList)
    result = eval(formula)
    
    print
    print player "'s " + year + "value: " + result
    
    
def player_lookup_sql(stat_type, player, year):
    if stat_type == "hit":
        relation = "player_hit"
    elif stat_type == "pitch":
        relation = 'player_pitch'
    else:
        relation = 'player_field'
    query = 'select * from ' + relation + ' where pName = %s' + ' and pYear = '\
        + year
    lookup = [player]
    cur.execute(query, lookup)
    tuples = []
    for Tuple in cur:
        tuples.append(Tuple)
    quit_input = raw_input("Enter anything to go back to main menu: ")

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
        print 'g) Build your own statistic'
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
            custom_stat()
        
        elif user_input == "q":
            user_input = "q"
            
        else:
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

#try:
    #connection = psycopg2.connect(database = "bskayton_baseball",\
                                  #user = "bskayton", password = 'bskayton')
        
#except StandardError, e:
    #print str(e)
    #exit

#cur= connection.cursor()