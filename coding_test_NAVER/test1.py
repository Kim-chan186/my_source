## 주어진 글자(문자열)의 조합들 중 중복 문자가 없는 가장 긴 문자열의 길이를 반환하는 매소드
## 최대 글자수 100

def find_overlap(_strs):
    strs = _strs.lower()
    for str in strs:
        if strs.count(str) != 1:
            ##print("end", strs, str)
            return 0
    ##print("Okay", strs)
    return len(strs) if len(strs) <= 100 else 100


def find_max_len(_strs):
    max_len = 0
    max_str = ""
    for str0 in _strs:
        str_len = find_overlap(str0)
        if str_len == 0:
            _strs.remove(str0)
        elif max_len < str_len:
            max_len = str_len
            max_str = str0
    return max_len, max_str, _strs


def addition_list(list1, list2):
    result_list = []
    for str1 in list1:
        for str2 in list2:
            if result_list.count(str1 + str2) == 0:
                result_list.append(str1+str2)
    return result_list


def all_addition_list(_list):
    list = _list[:]
    _list.append("")
    for i in range(len(_list)):

        list = addition_list(list, _list)
        list_buf = list[:]
        for str0 in list:
            if find_overlap(str0) == 0:
                list_buf.remove(str0)
                ##print("* pop", str0, list_buf)
        list = list_buf
        ##print("** list", list, _list)
    return list


def solution(A):
    A_list = list(A)
    max_len = 0
    max_str = ""

    max_len, max_str, A_list = find_max_len(A_list)
    A_list = all_addition_list(A_list)
    max_len, max_str, A_list = find_max_len(A_list)

    print("\n\n ** maximum length of Combination str is ** \n       ",  max_len)
    print("** maximum length is ** \n       ", max_str, A_list)


strs1 = {"co", "dil", "ity"}
strs2 = {"abc", "yyy", "def", "csv"}
strs3 = {"potato", "kayak", "banana", "racecar"}
strs4 = {"eva", "jqw", "tyn", "jan"}

solution(strs1)
solution(strs2)
solution(strs3)
solution(strs4)