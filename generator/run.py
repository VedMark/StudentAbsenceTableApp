from lxml import etree
from random import randint, choice
import sys


def get_data():
    global groupList
    global NAMES

    with open("data/MenSurname.txt") as data_file:
        menSurnameList = [elem[:-1] for elem in data_file]

    with open("data/MenName.txt") as data_file:
        menNameList = [elem[:-1] for elem in data_file]

    with open("data/MenPatronymic.txt") as data_file:
        menPatronymicList = [elem[:-1] for elem in data_file]

    with open("data/WomenSurname.txt") as data_file:
        womenSurnameList = [elem[:-1] for elem in data_file]

    with open("data/WomenName.txt") as data_file:
        womenNameList = [elem[:-1] for elem in data_file]

    with open("data/WomenPatronymic.txt") as data_file:
        womenPatronymicList = [elem[:-1] for elem in data_file]

    with open("data/Group.txt") as data_file:
        groupList = [elem[:-1] for elem in data_file]

    NAMES = dict([
        ('men', {
            'surname':
                menSurnameList,
            'name':
                menNameList,
            'patronymic':
                menPatronymicList
        }),
        ('women', {
            'surname':
                womenSurnameList,
            'name':
                womenNameList,
            'patronymic':
                womenPatronymicList
        })
    ])


def add_child_student(root_entry, student):
    student_child = etree.Element('entry')

    element = etree.Element('surname')
    element.text = student['surname']
    student_child.append(element)

    element = etree.Element('name')
    element.text = student['name']
    student_child.append(element)

    element = etree.Element('patronymic')
    element.text = student['patronymic']
    student_child.append(element)

    element = etree.Element('group')
    element.text = student['group']
    student_child.append(element)

    element = etree.Element('illness')
    element.text = str(student['illness'])
    student_child.append(element)

    element = etree.Element('another')
    element.text = str(student['another'])
    student_child.append(element)

    element = etree.Element('hooky')
    element.text = str(student['hooky'])
    student_child.append(element)

    root_entry.append(student_child)


def create_entries(root_entry, num_entries):
    for i in range(num_entries):
        sex = choice(['men', 'women'])
        student = dict([
            ('surname',     choice((NAMES[sex]['surname']))),
            ('name',        choice((NAMES[sex]['name']))),
            ('patronymic',  choice((NAMES[sex]['patronymic']))),
            ('group',       choice(groupList)),
            ('illness',     randint(0, 100)),
            ('another',     randint(0, 100)),
            ('hooky',       randint(0, 100))
        ])
        add_child_student(root_entry, student)

if __name__ == '__main__':
    count = int(sys.argv[2])
    get_data()

    root = etree.Element('studentData')
    create_entries(root, count)

    output_file = open(sys.argv[1], "w")
    output_file.write(etree.tostring(root, encoding='utf-8', pretty_print=True).decode("utf-8"))
    output_file.close()
