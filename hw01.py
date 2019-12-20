import bs4
import urllib.request

f = open("team(1).txt", "a", encoding= "utf8")
f01 = open("team(2).txt", "a", encoding= "utf8")
f02 = open("team(3).txt", "a", encoding= "utf8")
f03 = open("team(4).txt", "a", encoding= "utf8")

def func01():
    f.write('경북대학교 학교 학부사무실의 위치 및 연락처 찾는 프로그램\n')

    url01 = "http://cse.knu.ac.kr/01_sub/05_sub.html"
    html01 = urllib.request.urlopen(url01)

    soup = bs4.BeautifulSoup(html01, "html.parser")

    infos = soup.findAll("section", {"id":"locationCon"})

    f.write('학부 사무실 IT4호관 사무실 \n')
    for info in infos:
        tr_tag = info.findAll("tr")
        for tr_tag_one in tr_tag:
            f.write(tr_tag_one.text)
    f.write('\n')

def func02():
    f01.write('교수에 관한 정보를 찾아주는 프로그램 \n')
    url02 = "http://cse.knu.ac.kr/02_sub/01_sub.html"
    html02 = urllib.request.urlopen(url02)

    soup02 = bs4.BeautifulSoup(html02, "html.parser")

    member = soup02.find("ul", {"class":"member-list"})
    lis = member.findAll("li")


    for li in lis:
        p_tag = li.find("p")
        f01.write(p_tag.text)
        p_info = li.find("p", {"class":"txt"})
        f01.write(p_info.text)
    f01.write('\n')

def func03():
    f02.write('Abeek공지사항에 관한 정보를 찾아주는 프로그램 \n')

    url03 = "http://cse.knu.ac.kr/06_sub/02_sub_3.html"
    html03 = urllib.request.urlopen(url03)
    soup03 = bs4.BeautifulSoup(html03, "html.parser")

    articles = soup03.find("article", {"class":"sub-content area"})
    a_tags = articles.findAll("a")
    for a_tag in a_tags:
        f02.write('공지사항: ')
        f02.write(a_tag.text)
        f02.write('\n')
    f02.write('\n')

#a_tags = tr_tags.find("a")



def func04():
    f03.write('컴퓨터 학부 동아리에 관한 정보를 찾아주는 프로그램 \n')
    url04 = "http://cse.knu.ac.kr/06_sub/07_sub.html"
    html04 = urllib.request.urlopen(url04)

    soup04 = bs4.BeautifulSoup(html04, "html.parser")
    div = soup04.find("div", {"class":"transparent-list4"})
    lis = div.findAll("li")
    for li in lis:
        f03.write('동아리 이름: ')
        f03.write(li.text)
        f03.write('\n')
    f03.write('\n')

print('-----  홈페이지에서 찾을 수 있는 정보 텍스트 파일로 저장  -----\n\n')
def main():
    index = 1
    while(index):
        print('1. 위치 및 연락처 \n')
        print('2. 교수 정보 \n')
        print('3. Abeek관련 공지사항\n')
        print('4. 컴퓨터 학부 동아리 \n')
        print('종료를 원하시면 5를 누르세요 \n')

        choice = int(input('보고 싶은 정보를 선택하세요\n'))

        if choice == 1:
            print('위치 및 연락처에 관한 정보가 "team(01).txt"파일에 저장되었습니다. \n')
            func01()

        elif choice == 2:
            print('교수에 관한 정보가 "team(02).txt"파일에 저장되었습니다. \n')
            func02()

        elif choice == 3:
            print('Abeek에 관한 정보가 "team(03).txt"파일에 저장되었습니다. \n')
            func03()
        elif choice == 4:
            print('컴퓨터 학부 동아리에 관한 정보가 "team(04).txt"파일에 저장되었습니다. \n')
            func04()
        else:
            index = choice - 5
            print('프로그램을 종료합니다\n')

main()

