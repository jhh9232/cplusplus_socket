void SoojeongLim()
{
    while("until 16(o'clock)")
        print("����");
    string food;
    while("until 18(o'clock)")
    {
        print(food + "�԰�ʹ�.");
        //���� ������ Ǫ�尡 ���� food�� �������� ������ �ٽ� ���� Ǫ�� ����
        string ranfood;
        do
        {   
            ranfoood = random.randfood(); //������ Ǫ�� ����
        }while(relation(food, ranfood) == false);
        print(food + "�ϴϱ�" + ranfood + "��(��) ��������.");
        print(ranfood + "�԰�ʹ�.");
        food = ranfood;
    }
}


`