#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <list>

using namespace std;

vector<string> const STUDENT_NAMES {
    "Фамилия Имя",
    // …
};

vector<int> const TICKET_IDS {
    0,
    // …
};

size_t GetCyrillicStringLength(string const & string)
{
    int spaceCount = count(string.begin(), string.end(), ' ');

    return (string.length() - spaceCount) / 2 + spaceCount;
}

bool StringLengthComparer(string const & a, string const & b)
{
    return a.length() < b.length();
}

vector<string>::const_iterator GetLongestStudentName(vector<string> const & studentNames)
{
    return max_element(STUDENT_NAMES.begin(), STUDENT_NAMES.end(), StringLengthComparer);
}

void CreateStudentNameToTicketIdMap(
    vector<string> const & studentNames,
    vector<int> const & ticketIds,
    unordered_map<string, int> & result
)
{
    list<int> leftTicketIds(ticketIds.begin(), ticketIds.end());

    result = {};
    transform(
        studentNames.begin(),
        studentNames.end(),
        inserter(result, result.end()),
        [](string const & string) { return make_pair(string, -1); }
    );

    random_device randomDevice;
    mt19937 randomNumberGenerator(randomDevice());

    for (pair<string const, int> & pair : result)
    {
        if (leftTicketIds.empty())
        {
            leftTicketIds = list<int>(ticketIds.begin(), ticketIds.end());
        }
        uniform_int_distribution<> distribution(0, leftTicketIds.size() - 1);
        size_t randomTicketIdIndex = distribution(randomNumberGenerator);
        auto randomTicketIdIterator = leftTicketIds.begin();
        advance(randomTicketIdIterator, randomTicketIdIndex);
        int randomTicketId = *randomTicketIdIterator;
        pair.second = randomTicketId;
        leftTicketIds.erase(randomTicketIdIterator);
    }
}

int main(int argc, char * argv[])
{
    int longestStudentNameLength = GetCyrillicStringLength(*GetLongestStudentName(STUDENT_NAMES));

    unordered_map<string, int> studentNameToTicketIdMap;
    CreateStudentNameToTicketIdMap(STUDENT_NAMES, TICKET_IDS, studentNameToTicketIdMap);

    for (string const & studentName : STUDENT_NAMES)
    {
        int studentNameLength = GetCyrillicStringLength(studentName);
        string leftPaddingSpaces = string(longestStudentNameLength - studentNameLength, ' ');
        int studentTicketId = studentNameToTicketIdMap[studentName];
        cout << leftPaddingSpaces << studentName << " " << studentTicketId << endl;
    }

    return 0;
}
