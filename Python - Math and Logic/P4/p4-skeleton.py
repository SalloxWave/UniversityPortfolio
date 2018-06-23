# !usr/bin/env/ python
# -*- coding: utf-8 -*-

# Only used for testing, don't get the wrong idea ;)
import time, calendar, datetime



# TDP015 Inl�mningsuppgift 4

# F�rfattaren Lewis Carroll, mest k�nd f�r hans bock Alice i
# Underlandet, var �ven en beg�vad matematiker. Han utvecklade
# bl.a. en algoritm som ber�knar ett datums veckodag:
#
# http://www.cs.usyd.edu.au/~kev/pp/TUTORIALS/1b/carroll.html
#
# Er uppgift �r att implementera denna algoritm i Python. Resultatet
# ska vara en funktion day_of_the_week(y, m, d) som tar ett �rtal y
# (t.ex. 2016), en m�nad m (t.ex. 4 f�r april) och en dag d i denna
# m�nad och returnerar r�tt element fr�n f�ljande lista:


# Ni f�r inte anv�nda n�gra moduler, endast basfunktioner. (Carroll's
# beskrivning skiljer mellan old style och new style datum. Ni beh�ver
# bara hantera new style datum.)



month_items = {}
DAYS_OF_MONTH = {1:31, 2:28, 3:31, 4:30, 5:31, 6:30, 7:31, 8:31, 9:30, 10:31, 11:30, 12:31}
DAYS = ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat']

def init_month_items():
    for month in range(1,13):
        if month == 1: # Jan
            month_items[month] = 0
        elif month == 2 or month == 3: # Feb & Mar
            month_items[month] = 3
        elif month == 12: # Dec
            month_items[month] = 12 % 7
        else:
            month_items[month] = (month_items[month-1] + DAYS_OF_MONTH[month-1]) % 7


def old_style(y, m, d):
    if y <  1752 or \
       y == 1752 and m < 9 or \
       y == 1752 and m == 9 and d <= 2:
        return True
    return False

    ''' ALTERNATIVE SOLUTION
    if y == 1752:
        if m == 9:
            if d <=2:
                return True
    elif y < 1752:
        return True
    return False
    '''

def must_be_corrected(y, m, d):
    if y%4==0 and m in range(1,3):
        if (not old_style(y, m, d)) and y%100==0:
            if int(str(y)[:2])%4==0:
                return True
            return False
        return True
    return False


def day_of_week(y, m, d):
    century = int(str(y)[:2])
    year_over = int(str(y)[2:])

    century_item = 0
    year_item = 0
    month_item = 0
    day_item = 0

    if debug: print("")

    ''' CENTURY CALCULATIONS '''
    # old style
    if old_style(y, m, d):
        century_item = 18-century
    # new style
    else:
        century_item = (3-(century%4))*2

    if debug: print("Century-item: {}".format(century_item))

    ''' YEAR CALCULATIONS '''
    year_item = (year_over // 12) + (year_over % 12) + ((year_over % 12) // 4)
    if debug: print("Year-item: {}".format(year_item))
    
    ''' MONTH CALCULATIONS '''
    month_item = month_items[m]
    if debug: print("Month-item: {}".format(month_item))

    ''' DAY CALCULATIONS '''
    day_item = d
    if debug: print("Day-item: {}".format(day_item))

    ''' FINAL (almost) '''
    result = century_item + year_item + month_item + day_item
    result = result % 7
    if debug: print("FINAL RESULT: {}".format(result))

    ''' Adjust for Leap Years and return '''
    # Read at your own risk
    return DAYS[(6 if result==0 else result-1) if must_be_corrected(y, m, d) else result]

    ''' Awesome Alternative Solutions

    if must_be_corrected(y,m,d): return DAYS[result+6] if result==0 else DAYS[result-1]
    return DAYS[result]

    if must_be_corrected(y,m,d):
        if result==0:
            return DAYS[result+6]
        else:
            return DAYS[result-1]
    return DAYS[result]

    '''
        


# Configurations
debug = False
init_month_items()
if debug: print("Month-items: {}".format(month_items))

# Tests
print("")
print("Today is a " + day_of_week(int(time.strftime("%Y")), int(time.strftime("%m")), int(time.strftime("%d"))) + " (= "+calendar.day_name[datetime.datetime.today().weekday()]+")")
print("")
print("================ Random test ================")
print("18 Sep 1783 is a " + day_of_week(1783, 9, 18) + " (= Thursday)")
print("23 Feb 1676 is a " + day_of_week(1676, 2, 23) + " (= Wednesday)")
print("")
print("============== Leap Year test ===============")
print("18 Feb 2016 is a " + day_of_week(2016, 2, 18) + " (= Thursday)")
print("5 Jan 2000 is a " + day_of_week(2000, 1, 5) + " (= Wednesday)")
print("4 Jan 1800 is a " + day_of_week(1800, 1, 4) + " (= Saturday)")
print("")
print("============ Vecka 17, 2017 test ============")
print("24 April 2017 is a " + day_of_week(2017, 4, 24) + " (= Monday)")
print("25 April 2017 is a " + day_of_week(2017, 4, 25) + " (= Tuesday)")
print("26 April 2017 is a " + day_of_week(2017, 4, 26) + " (= Wednesday)")
print("27 April 2017 is a " + day_of_week(2017, 4, 27) + " (= Thursday)")
print("28 April 2017 is a " + day_of_week(2017, 4, 28) + " (= Friday)")
print("29 April 2017 is a " + day_of_week(2017, 4, 29) + " (= Saturday)")
print("30 April 2017 is a " + day_of_week(2017, 4, 30) + " (= Sunday)")
print("=============================================")


# Ledning
#
# Carrolls egen beskrivning av algoritmen �r formulerad som en g�ta.
#
# Den del som kanske �r sv�rast att f�rst� �r "The Month-item":
#
# 'If it begins or ends with a vowel, subtract the number, denoting
# its place in the year, from 10. This, plus its number of days, gives
# the item for the following month. The item for January is "0"; for
# February or March, "3"; for December, "12".'
#
# Detta betyder allts� att talen f�r januari (0), februari (3), mars
# (3) och december (12) �r direkt givna; och att talen f�r en m�nad
# vars namn (p� engelska) inte b�rjar eller slutar med en vokal kan
# r�knas ut fr�n f�reg�ende m�nads "Month-item" genom att man plussar
# p� antalet dagar som denna f�reg�ende m�nad har.
#
# Det som kanske �r minst tydligt �r hur talen f�r de resterande
# m�naderna ska r�knas ut, dvs. m�nader vars namn b�rjar eller slutar
# med en vokal. Men det st�r egentligen explicit i f�rsta meningen:
#
# 'If it begins or ends with a vowel, subtract the number, denoting
# its place in the year, from 10.'
#
# Detta ger allts� t.ex. 6 f�r april eftersom 10 - 4 = 6.
#
# Med detta s� kan ni allts� r�kna ut alla "Month-items". Mitt f�rslag
# �r att ni r�knar ut dem endast en g�ng och sedan helt enkelt lagrar
# dem i en lista eller liknande.
