#include "Proccessing.h"

Proccessing::Proccessing(Phonotext pt, std::string lng, double min_pwr, double max_pwr)
{
    std::cout << "proccess start\n";
    this->pt = pt;
    this->min_pwr = min_pwr;
    this->max_pwr = max_pwr;

    std::cout << "Add config\n";
    Conf CONF(lng);
    //this->CONFIG = CONF;
    std::cout << "Config added\n";

    proccess();
}

// ������ ��������� ������
void Proccessing::proccess()
{

    std::cout << "modify\n";
    modifyProccessor(); // ������
    std::cout << "same\n";
    sameProccessor(); // ������ � ������
    std::cout << "join\n";
    joinProccessor(); // �����������
    std::cout << "number\n";
    numberProccessor(); // �������
    std::cout << "finder\n";
    finderVolve(); // �����
    std::cout << "SP\n";
    SPmaxProccessor(); // �������
    std::cout << "combinations\n";
    combinationsProccessor(); // ��������������
    std::cout << "repeat\n";
    repeatProccessor(); // �������
    std::cout << "proccessing end\n";
}


// ��������� �������� �� json
// ���� ��������, �� ������ �������� � ��������������
void Proccessing::modifyProccessor()
{
    std::string tmp_a;
    std::string tmp_b;
    std::string tmp_c;

    auto itPreviosLetter = pt.basetext.begin();

    bool needChange = false;
    for (auto it = pt.basetext.begin(); it != pt.basetext.end(); it++)
    {
        if (needChange)
        {
            // it->printable = "`";
            needChange = false;
        }
        if (it == pt.basetext.begin())
        {
            tmp_b = it->origin;
            std::cout << tmp_b << std::endl;
        }
        else
        {
            tmp_a = tmp_b;
            tmp_b = it->origin;


            std::cout << "Modify config start\n";
            auto modFirstKey = CONFIG.getModifications().find(tmp_a);
            std::cout << "Modify config end\n";
            if (modFirstKey != CONFIG.getModifications().end())
            {
                std::cout << "is here\n";
                int i, l;
                auto modSecondKey = modFirstKey->second.find(tmp_b);
                if (modSecondKey != modFirstKey->second.end())
                {

                    std::cout << "config start using\n";
                    tmp_c = modSecondKey->second;
                    std::cout << "config end using\n";
                    for (i = 0; tmp_c[0] & (0x80 >> i); ++i); i = (i) ? i : 1; // find first letter
                    for (l = 0; tmp_c[i] & (0x80 >> l); ++l); l = (l) ? l : 1; // find second letter

                    itPreviosLetter->origin = tmp_c.substr(0, i);
                    it->origin = tmp_c.substr(i, l);
                    pt.basetext.emplace_after(it, Letter(tmp_c.substr(i + l)));
                    auto b = it;
                    (++b)->printable = it->printable;
                    it->printable = "`";
                    needChange = true;
                }
            }

            std::cout << "haven't if\n";
        }
        itPreviosLetter = it;
    }
}


// ������ technic � printable, � ��� �� ������ �������� � ������, ���� ��� �������� ����������� �� json
// P.S. ����� ������ �� ��������
void Proccessing::sameProccessor()
{
    for (auto& symb : pt.basetext)
    {
        auto it = CONFIG.getAsSame().find(symb.printable);
        if (it != CONFIG.getAsSame().end())
            symb.technic = it->second;
        else if (symb.technic == "+")
            symb.technic = "&";

        if (symb.origin == " ")
            symb.technic = "|";

        // if (symb.technic == "&" || symb.technic == "|")
            // symb.printable = symb.technic;

        if (symb.technic == "&")
            symb.technic = symb.origin;
    }
}

// ����������� ��������, ���� ��� �������� ����� ��������
void Proccessing::joinProccessor()
{
    std::string tmp_a; // ������ ������ ������������� � ����������
    std::string tmp_b; // ������ ������ ������������� � ����������

    // ����� ��������� �� ��������� ������ � Phonotext
    auto itLast = pt.basetext.begin();
    for (auto it = pt.basetext.begin(); it != pt.basetext.end(); it++)
        itLast = it;

    auto it = pt.basetext.begin(); // ������ ���������� ����� ���������� �� ������, � ������� ���������� ������ � ������ ������
    auto itPreviosLetter = pt.basetext.begin(); // ������ ���������� ����� ���������� �� ������, � ������� ����������� ������ � ������� ���

    while (it != pt.basetext.end()) // �������� �� �����
    {
        if (it == pt.basetext.begin()) // � ������ ��� ����������� ������ ������ ������ �������� ����������
            tmp_a = it->origin;
        else
        {
            tmp_a = itPreviosLetter->origin; // ������ ������� ������� (origin �� ����������� ���������)
            tmp_b = it->origin; // ������ ������� ������� (origin �� ���������� �������)

            auto sameKey = CONFIG.getAsOne().find(tmp_a[0]); // ����� ���������� �� ������� �������
            if (sameKey != CONFIG.getAsOne().end()) // ���� ���������� �������
            {
                if ((tmp_a + tmp_b) == sameKey->second) // ���� ���� ���� ��������� � �����������
                {
                    // ���������� �������� ���������
                    itPreviosLetter->origin = sameKey->second;
                    itPreviosLetter->printable = sameKey->second;
                    itPreviosLetter->technic = sameKey->second;
                    if (it == itLast) // ���� ��� ��������� �������, �� �������� � �����
                    {
                        pt.basetext.erase_after(itPreviosLetter);
                        break;
                    }
                    // ���� ��� �� ��������� �������, �� �������� �� ��������� ��������, �� ��������� ������ � �������� ���������� �������
                    it++;
                    pt.basetext.erase_after(itPreviosLetter);
                }
            }
            itPreviosLetter = it; // ����� �������� ��������� �� ���������
        }
        it++; // ��������� ��������
    }
}

// ������� ��� �������� ��������� ���������� �� ������
void Proccessing::numberProccessor()
{
    int i = 0; // ��� �������� ������ ����� � ������
    int j = 1; // ��� �������� ����� � ������
    int k = 1; // ��� �������� ����� � ������
    int num = 1; // �� ������������, �� � ��������� ����
    bool space = false; // ���� ����������, �������� �� ������ ����������� ������ ��������
    int space_pos = 0; // ��� �������� ���������� ������� � ����� ����� �������

    int l_number = 0; // �� ������������, �� � ��������� ����
    for (auto it = pt.basetext.begin(); it != pt.basetext.end(); it++, l_number++)
    {
        it->number = i;
        it->word = k;

        if (it->technic == " " || it->technic == "|" || it->technic == "\n")
        {
            space_pos = 0;
            space = true;
        }
        else
        {
            std::vector<std::string> words = CONFIG.getWords();
            space_pos++;
            // ��� ���� �� �������� ������ �� ������ �����, �� ��-����� ��� ������ �� ������
            bool flag = false;
            for (int t = 0; t < words.size(); t++)
                if (it->technic == words[t])
                    flag = true;
            space = (space && flag);
            // �� ����� �������
        }
        if (it->technic == "\n")
        {
            // ���������
            // P.S. �� ������ � �� ��������� ��������� ��� �����, �� ����� �� ��������
        }
        // ����������� ������� ��������
        std::vector<std::string> volves = CONFIG.getVolves();
        for (int t = 0; t < volves.size(); t++)
            if (it->printable == volves[t])
            {
                it->isVolve = true;
                j++;
                break;
            }
        // ����������� ��������� ��������
        std::vector<std::string> consonants = CONFIG.getConsonants();
        for (int t = 0; t < consonants.size(); t++)
            if (it->printable == consonants[t])
            {
                it->isConsonant = true;
                break;
            }
        it->syll = j;
        i++;
        num++;
        it->w_pos = space_pos; // ��������� ������ � ����, � ������, �������� �� �����������
    }
}

// ���������� ���������� ������� � ���������� ������� ������� �� ����� ����� �����������
void Proccessing::finderVolve()
{
    for (auto it = pt.basetext.begin(); it != pt.basetext.end(); it++)
        if (it->isVolve)
            volveIterators.push_back(it);
}

// �������� ������� ���������, ������� ������������ ����� 
// ��������� �� ��������� ��������� ����� ������� ������ � ��������� ��������� ����� ��������� �������
void Proccessing::SPmaxProccessor()
{
    std::vector<std::pair<std::forward_list<Letter>::iterator, std::forward_list<Letter>::iterator>> dividedVolveIterators;

    std::forward_list<Letter>::iterator startVolveIt, middleVolveIt, endVolveIt; // ��������� �� �������, ��������� � ��������� �������
    bool firstVolve = false;

    if (volveIterators.size() > 1) // ���� ������� ������ �����
    {
        // �������������� ������������ ��������� � SP - ������ ������, �.�. ������ - ������ �� ���������
        startVolveIt = pt.basetext.begin();
        startVolveIt++;
        // �������� �� ������ ����� = �������� �� ������ �������
        middleVolveIt = volveIterators[0];

        for (int i = 1; i < volveIterators.size(); i++) // ������ �� ���� ���������� �������, ������� �� �������
        {
            endVolveIt = volveIterators[i]; // �������� �������� �������� �������, ����������� �� ��������� ��������� ����� ��������� �������

            dividedVolveIterators.push_back(std::make_pair(startVolveIt, endVolveIt)); // �������� ���� �� ��������� ��������� ����� ��������� ������� � ��������� ��������� ����� ��������� �������

            // ��������� ������ - ��������� ������� ����� ���, ������� �������� ����������� �� ������ ������
            // ������������� ������� � �������������
            middleVolveIt++;
            startVolveIt = middleVolveIt;
            // ��������� ����������� - ��, ������� �������� ��������� �� ������ �����
            middleVolveIt = endVolveIt;
            // ������ ��� ������� �� ��������� �� ��������
        }

        // ���������� �������� �������, ������� �������� ��������� ����� ��������� �������
        endVolveIt = pt.basetext.end();
        startVolveIt++;
        dividedVolveIterators.push_back(std::make_pair(startVolveIt, endVolveIt));
    }
    else // ���� ������� ���� - ������� � ������� ������ ����: ���� Phonotext
        dividedVolveIterators.push_back(std::make_pair(pt.basetext.begin(), pt.basetext.end()));

    pt.SP = dividedVolveIterators; // ������� ���� ��������� � Phonotext
}

// ���������� �������� ������� � ���������
std::pair<int, std::vector<int>> Proccessing::findLocalWordsInds(std::pair<std::forward_list<Letter>::iterator, std::forward_list<Letter>::iterator> localSP)
{
    int indVolve;
    std::vector<int> indCons;

    int i = 0;
    for (auto it = localSP.first; it != localSP.second; it++, i++)
        if (it->isVolve)
            indVolve = i;
        else
            indCons.push_back(i);

    return std::make_pair(indVolve, indCons);
}

// �������� ����������
void Proccessing::combinationsProccessor(int N)
{
    N++;
    for (int i = 0; i < pt.SP.size(); i++)
    {
        // �� ������� ������� ���������� ������� � ��� ��������� � ��������� ������
        std::pair<int, std::vector<int>> localInds = findLocalWordsInds(pt.SP[i]);
        int posVolve = localInds.first;
        std::vector<int> posCons = localInds.second;

        // ������������ ���� ���������� �� �������� ������� ���������
        std::vector<std::pair<int, int>> consCombs;
        for (int j = 0; j < posCons.size(); j++)
            for (int k = j + 1; k < posCons.size(); k++)
                consCombs.push_back(std::make_pair(posCons[j], posCons[k]));

        // ������ ���������� �� ��� � ������ ��������� (� ����������)
        std::vector<std::vector<std::forward_list<Letter>::iterator>> itCombs;
        for (int j = 0; j < consCombs.size(); j++)
        {
            // �������� ������������� ���������� �� ��� �������� ���������� + ������ �������
            std::vector<int> combs;
            combs.push_back(consCombs[j].first);
            combs.push_back(consCombs[j].second);
            combs.push_back(posVolve);
            sort(combs.begin(), combs.end());

            // ����� ���������� ��������� ��������� � ����
            std::forward_list<Letter>::iterator it1, it2, it3;
            it1 = it2 = it3 = pt.SP[i].first;

            for (int k = 0; k < combs[0]; k++)
                it1++;
            for (int k = 0; k < combs[1]; k++)
                it2++;
            for (int k = 0; k < combs[2]; k++)
                it3++;

            std::vector<std::forward_list<Letter>::iterator> tmpItCombs;
            tmpItCombs.push_back(it1);
            tmpItCombs.push_back(it2);
            tmpItCombs.push_back(it3);
            itCombs.push_back(tmpItCombs);
        }
        pt.syllableCombinations.push_back(itCombs);

        // �������� ��, ��� ������������� �� ���������� � ��������� �� filter_combination
    }
}

// �������, ������� ������������ ���������� ���������� ���������� � �� ����
void Proccessing::repeatProccessor()
{
    for (int n_syll = 0; n_syll < pt.syllableCombinations.size(); n_syll++)
    {
        for (auto& comb : pt.syllableCombinations[n_syll])
        {
            std::pair<std::vector<std::string>, std::map<int, int>> a;

            // �������� ��������� Repeat, � ������� ������������� ���������� � ���� � ��� (���������� ������� �� ��: ������� + �������)
            for (auto& i : comb)
            {
                if (i->isConsonant)
                    a.first.push_back(i->technic);
                a.second.insert(std::make_pair(i->number, n_syll)); // ���������� ��������� �� ������������ � ����������
            }

            // �������� ������������� ��������
            std::set<std::string> tmpWords(a.first.begin(), a.first.end());
            std::string setToStr = "";
            for (auto& i : tmpWords)
                setToStr += i;

            // ������������ ���������� ����������
            std::pair<bool, double> filter = rusFilterComb(comb, CONFIG.getWords());
            if (filter.first)
            {
                std::map<std::string, Repeat>::iterator it = pt.repeats.find(setToStr); // �������� �� ����������� ������ ���������� � ��� �����������
                if (it == pt.repeats.end()) // �������� ��������� Repeat, ���� ��������� � ����� ���������� ��� ������
                {
                    Repeat tmpRepeat;
                    tmpRepeat._words = tmpWords; // ���������� ������� ���������
                    tmpRepeat.count = 1; // ��������� ������ ��������, ������������� ��� ������ ������ �����������
                    tmpRepeat.power = filter.second; // ���������� ���� ������ ����������� ����������

                    std::vector<Letter> tmpvector;
                    for (auto& i : comb) // ���������� ���� �������� ������ Letter, ������� ������������ � ���������� �� ��������� ������
                        tmpvector.push_back(*i);
                    for (int i = 0; i < tmpvector.size(); i++) // ���������� ���������� �������� ������ Letter �� ���������� ������� � ���������
                    {
                        bool flag = false;
                        for (int j = 0; j < tmpRepeat.letters.size(); j++)
                            if (tmpvector[i] == tmpRepeat.letters[j])
                                flag = true;
                        if (!flag)
                            tmpRepeat.letters.push_back(tmpvector[i]);
                    }

                    tmpRepeat.combs.push_back(comb); // ���������� ���� ���������� � ���������

                    pt.repeats.insert(std::make_pair(setToStr, tmpRepeat)); // ������� ��������� � ������ Phonotext
                }
                else
                {
                    it->second._words = tmpWords; // ���������� ������� ���������
                    it->second.count += 1; // ��� ��� ������� ��� ���� ����������, ������� �������������
                    it->second.power = filter.second; // ���������� ���� ��������� ����������� ����������

                    std::vector<Letter> tmpvector; // ���������� ������� ���������
                    for (auto& i : comb)
                        tmpvector.push_back(*i);
                    for (int i = 0; i < tmpvector.size(); i++) // ���������� ���������� �������� ������ Letter �� ���������� ������� � ���������
                    {
                        bool flag = false;
                        for (int j = 0; j < it->second.letters.size(); j++)
                            if (tmpvector[i] == it->second.letters[j])
                                flag = true;
                        if (!flag)
                            it->second.letters.push_back(tmpvector[i]);
                    }

                    it->second.combs.push_back(comb); // ���������� ���� ���������� � ���������
                }
            }
        }
    }
}

// ������� ������� ��� ���������� ����������, ���������� ������ ���������� <false, 0>, ���� ���������� �������� �������������,
// ���������� <true, pwr>, ���� ���������� �������, ��� pwr - ���� ���������� � ������
// ����������: ������� �������� �� ��, ��� �����, ���������� �� ���������� �������� ���������������
std::pair<bool, double> Proccessing::rusFilterComb(std::vector<std::forward_list<Letter>::iterator> comb, std::vector<std::string> words)
{
    // ������ technic �������� ����������
    std::string tmptxt = "";
    for (auto it = comb[0]; it != comb[2]; it++)
        tmptxt += it->technic;

    // ������������ ������������� ��������
    std::string txt = "";
    for (int i = 0; i < tmptxt.size(); i++)
    {
        bool notUnique = false;
        for (int j = 0; j < txt.size(); j++)
            if (tmptxt[i] == txt[j])
                notUnique = true;

        if (!notUnique)
            txt += tmptxt[i];
    }

    // ���������� �� �������, ���� ���������� ����� ������ ��� ��������
    if (txt.size() < 3)
        return std::make_pair(false, 0);

    // ������� ���� ����������
    double pwr;
    if (comb[0]->isVolve) // ���� ������� ������ � ����������
        pwr = 2;
    else if (comb[2]->isVolve) // ���� ������� ��������� � ����������
        pwr = 1;
    else // ���� ������� � �������� ����������
        pwr = 3;

    int count = 0;
    for (auto& i : txt) // ������� ���������� ��������
        if (i == '|')
            count++;
    pwr += (comb[2]->number - comb[0]->number - count == 2 ? 5 : 0); // ������������ ������� ��� ��������
    pwr += (count == 0 ? 2 : 0); // ���������� ��������
    count = 0;
    for (int i = txt.size() - 3; i < txt.size(); i++) // ������� ���������� ���� "�" � ��������� ��� �������� ����������� ������
        if (txt[i] == '�')
            count++;
    pwr += (count == 0 ? 4 : 0);
    pwr += (!comb[0]->w_pos || !comb[1]->w_pos || !comb[2]->w_pos ? 1 : 0); // �������� �� ��, �������� �� ������� �� ���������� ������ � �����
    pwr /= 15;

    return std::make_pair(min_pwr <= pwr <= max_pwr, pwr);
}

// �����
void Proccessing::print()
{
    std::cout << "===========\n";

    std::cout << "-----------\n";
    std::cout << "origin      : ";
    for (auto& i : pt.basetext)
    {
        std::cout << i.origin;
    }
    std::cout << std::endl;

    std::cout << "-----------\n";
    std::cout << "technic     : ";
    for (auto& i : pt.basetext)
    {
        std::cout << i.technic;
    }
    std::cout << std::endl;

    std::cout << "-----------\n";
    std::cout << "printable   : ";
    for (auto& i : pt.basetext)
    {
        std::cout << i.printable;
    }
    std::cout << std::endl;

    std::cout << "-----------\n";
    std::cout << "isWord      : ";
    for (auto& i : pt.basetext)
    {
        if (i.isVolve)
            std::cout << "v";
        else if (i.isConsonant)
            std::cout << "c";
        else
            std::cout << "n";
    }
    std::cout << std::endl;

    std::cout << "-----------\n";
    std::cout << "w_pos      : ";
    for (auto& i : pt.basetext)
    {
        std::cout << i.w_pos;
    }
    std::cout << std::endl;

    std::cout << "-----------\n";
    std::cout << "SP          :\n";
    for (int i = 0; i < pt.SP.size(); i++)
    {
        std::cout << i << ": \"";
        for (auto it = pt.SP[i].first; it != pt.SP[i].second; it++)
        {
            std::cout << it->technic;
        }
        std::cout << "\"" << std::endl;
    }
    std::cout << std::endl;
    for (auto& i : pt.basetext)
    {
        std::cout << i.origin << i.technic << i.printable << i.isVolve << i.isConsonant << i.number << '|';
    }

    std::cout << "-----------\n";

    std::cout << "combinations:\n";
    for (int i = 0; i < pt.syllableCombinations.size(); i++)
    {
        std::cout << i << ":\n";
        for (int j = 0; j < pt.syllableCombinations[i].size(); j++)
        {
            for (int k = 0; k < 3; k++)
                std::cout << pt.syllableCombinations[i][j][k]->origin;
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "-----------\n";
    std::cout << "repeats:\n";
    for (auto& i : pt.repeats)
    {
        std::cout << "key : " << i.first << std::endl;
        std::cout << "Repeat.power : " << i.second.power << std::endl;
        std::cout << "Repeat.count : " << i.second.count << std::endl;
        std::cout << "Repeat.letters : ";
        for (int j = 0; j < i.second.letters.size(); j++)
            std::cout << i.second.letters[j].origin;
        std::cout << std::endl << "Repeat._words : <";
        for (auto& j : i.second._words)
            std::cout << j << "><";
        std::cout << ">\n";
        std::cout << "Repeat.combs : ";
        for (int j = 0; j < i.second.combs.size(); j++)
        {
            for (int k = 0; k < i.second.combs[j].size(); k++)

                std::cout << i.second.combs[j][k]->origin;
            std::cout << " ";
        }
        std::cout << std::endl << std::endl;
    }
    std::cout << std::endl;
    std::cout << "-----------\n";

}
