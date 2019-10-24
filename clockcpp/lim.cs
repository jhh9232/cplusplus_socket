void SoojeongLim()
{
    while("until 16(o'clock)")
        print("졸려");
    string food;
    while("until 18(o'clock)")
    {
        print(food + "먹고싶다.");
        //랜덤 생성한 푸드가 기존 food와 연관되지 않으면 다시 랜덤 푸드 생각
        string ranfood;
        do
        {   
            ranfoood = random.randfood(); //랜덤한 푸드 생각
        }while(relation(food, ranfood) == false);
        print(food + "하니까" + ranfood + "이(가) 생각나네.");
        print(ranfood + "먹고싶다.");
        food = ranfood;
    }
}


`