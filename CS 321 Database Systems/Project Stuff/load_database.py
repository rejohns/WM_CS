#!/usr/bin/python
import psycopg2
import getpass
import sys
import csv
def main():
    try:
        connection = psycopg2.connect(database= "bskayton_baseball", user= "bskayton", password= getpass.getpass())
    
    except StandardError, e:
        print str(e)
        exit
   
    cur= connection.cursor()
    
    change_year=0
    year=0
    team=""
    change_team=0
    age= 27
    upload_player_stat= []

    #changing pitchin, batting, fielding, all we need to do is change the int and
    #floats; changing the check_pitching, check_fielding, check_batting; players_pitching.csv, 
    # players_fielding, players_batting, team_pitching, teamp_fielding, team_batting?
    #with open('players_pitching.csv', 'rb') as csvfile:
    with open('players_hitting.csv', 'rb') as csvfile:
        row_reader= csv.reader(csvfile, delimiter=',', quotechar='|')
        for row in row_reader:
            i=0
            j=0
            stat=0
            stat_2= 0.0
            position=""
            name=""                    
            for j in range(len(upload_player_stat)):
                upload_player_stat.pop()
            for entry in row:
                temp_1= entry.replace(',', ' ')
                temp_2= temp_1.replace('*', '')
                temp_3= temp_2.replace('#', '')
                mystr= temp_3.replace('?', '') 
                #temp_4= temp_3.replace('?', '')
                #mystr= temp_4.replace('%', '')
                if(change_team==1):
                    team= mystr
                    change_team=0
                    print team                
                elif(change_year==1):
                    year= int(mystr)
                    change_year=0
                    print year                
                elif(mystr== "break_team"):
                    change_team=1                               
                elif(mystr== "break_year"):
                    change_year=1
                #elif(check_str_pitching(mystr)==True):
                #elif(check_str_batting(mystr)==True):
                elif(mystr== "Rk"):
                    break
                else:
                    if(i!=0):
                        #if(i==0):
                        #    name= mystr
                        if(i==1):
                            if(mystr==""):
                                position= None
                            else:
                                position= mystr
                            #age= int(mystr)
                        elif(i==2): #out-fielding
                            name= mystr# out- fielding
                        elif(i==3): #out-fielding
                            age= int(mystr) #out-fielding
                        else:
                            if(mystr==""):
                                upload_player_stat.append(None)
                            else:
                                #if(i==4 or i==5 or (i>8 and i<14) or (i>15 and i<27)):
                                if((i>3 and i<15) or i>20): #batting
                                #if((i>1 and i<4) or (i>5 and i<10) or (i>11 and i<15) or (i>17 and i<24)
                                    stat= int(mystr)
                                    upload_player_stat.append(stat)
                                else:
                                    #if(i== 25): #fielding
                                    #    position= mystr
                                    #else:
                                    stat_2= float(mystr)
                                    upload_player_stat.append(stat_2)
                                #print mystr
                    i= i+1
            if(len(upload_player_stat)>0):
                #print upload_player_stat
                upload_player_stat.insert(0, age)
                upload_player_stat.insert(0, name)
                upload_player_stat.insert(0, position)
                upload_player_stat.insert(0, year)
                upload_player_stat.insert(0, team)
                #print upload_player_stat
                #cur.execute('INSERT INTO player_hit VALUES(%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)', upload_player_stat)
                #cur.execute('INSERT INTO player VALUES(%s, %s, %s, %s', upload_player_stat)
                #cur.execute('INSERT INTO player_pitch VALUES(%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)', upload_player_stat)
                
    
    
    
    connection.commit()
    cur.close()
    connection.close()    
    exit

def check_str_pitching(tempStr):
    if(tempStr== "Rk" or tempStr=="Pos" or tempStr=="Age" or tempStr== "W" or tempStr== "L" or tempStr=="W-L%" or tempStr=="ERA" or tempStr=="G" or tempStr=="GS" or tempStr=="GF" or tempStr=="CG" or tempStr=="SHO" or tempStr=="SV" or tempStr=="IP" or tempStr=="H" or tempStr=="R" or tempStr=="ER" or tempStr=="HR" or tempStr=="BB" or tempStr=="IBB" or tempStr=="SO" or tempStr=="HBP" or tempStr=="BK" or tempStr=="WP" or tempStr=="BF" or tempStr=="ERA+" or tempStr=="WHIP" or tempStr=="H/9" or tempStr=="HR/9" or tempStr=="BB/9" or tempStr=="SO/9" or tempStr=="SO/BB"):
        return False
    else:
        return True

def check_str_fielding(tempStr):
    if(tempStr=="Age" or tempStr=="G" or tempStr=="PA" or tempStr=="Rbat" or tempStr=="Rbaser" or tempStr=="Rdp" or tempStr=="Rfield" or tempStr=="Rpos" or tempStr=="RAA" or tempStr=="WAA" or tempStr=="Rrep" or tempStr=="RAR" or tempStr=="WAR" or tempStr=="waaWL%" or tempStr=="162WL%" or tempStr=="oWAR" or tempStr=="dWAR" or tempStr=="oRAR" or tempStr=="Salary" or tempStr=="Acquired" or tempStr=="Pos Summary"):
        return False
    else:
        return True

def check_str_batting(tempStr):
    if(tempStr=="Rk" or tempStr=="Pos" or tempStr=="Age" or tempStr=="G" or tempStr=="PA" or tempStr=="AB" or tempStr=="R" or tempStr=="H" or tempStr=="2B" or tempStr=="3B" or tempStr=="HR" or tempStr=="RBI" or "SB" or tempStr=="CS" or tempStr=="BB" or tempStr=="SO" or tempStr=="BA" or tempStr=="OBP" or tempStr=="SLG" or tempStr=="OPS" or tempStr=="OPS+" or tempStr=="TB" or tempStr=="GDP" or tempStr=="HBP" or tempStr=="SH" or tempStr=="SF" or tempStr=="IBB"):
        return False
    else:
        return True
    

main()
