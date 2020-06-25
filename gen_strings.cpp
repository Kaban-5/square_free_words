bool bad_string (const string &s)
{
    for (int32_t len = 1; 2 * len < int32_t(s.size()); len++)
    {
        for (int32_t i = 0; i + 2 * len <= int32_t(s.size()); i++)
        {
            if (equal(s.begin() + i, s.begin() + i + len, s.begin() + i + len))
                return true;
        }
    }

    return false;
}

bool bad_double_string (const string &s)
{
    return bad_string(s + s);
}

void generate_and_output (string s, ostream &out, int64_t &cnt) {
    if (!s.empty() && !bad_double_string(s))
    {
        out << s << endl;
        cnt++;
    }

    if (int(s.size()) == necessary_dictionary_word_length)
        return;

    for (char ch = 'a'; ch <= 'c'; ch++)
    {
        s += ch;
        bool ok = true;
        for (int32_t i = 1; ok && 2 * i <= int32_t(s.size()); i++)
            ok &= !equal(s.end() - 2 * i, s.end() - i, s.end() - i);

        if (ok)
            generate_and_output(s, out, cnt);
        s.pop_back();
    }
}
