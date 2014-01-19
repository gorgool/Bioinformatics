#include <vector>
#include <string>
#include <sstream>

using namespace std;

template<typename T, char ... delims>
vector<T> split_string(const string& input)
{
  static char _delims[] = {delims ...};
  struct tokens : ctype<char>
  {
    tokens() : ctype<char>(get_table()) {}

    static ctype_base::mask const* get_table()
    {
      typedef ctype<char> cctype;
      static const cctype::mask *const_rc = cctype::classic_table();

      static cctype::mask rc[cctype::table_size];
      memcpy(rc, const_rc, cctype::table_size * sizeof(cctype::mask));

      for (char del : _delims)
      {
        rc[del] = ctype_base::space;
      }

      return &rc[0];
    }
  };

  stringstream ss(input);
  ss.imbue(locale(locale(), new tokens()));
  return vector<T>(istream_iterator<T>{ ss }, istream_iterator<T>());
}