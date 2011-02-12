#include <cstdio>
#include <cstring>
#include <assert.h>

// These are BIG log files! Thankfully you remembered to compile me 64-bit right?

FILE* file;
// Small for testing purposes, enlarge for speed.
// In all, the ideal size is going to be big enough so that on average we catch
// a date in a single fseek, but not much more. The performance is highly data
// dependant.
const size_t buffer_size = 10;
char buffer[buffer_size*2+1];
size_t read_len;
char* curr;
off_t buffer_start;

//TODO: scan the file to detmine this:
int start_day = 9;

// In retrospect I should probally just invert all of these for clarity, at the
// cost of typing a bit more.
typedef bool ebool;

void fill_buffer()
{
    buffer_start = ftello(file);
    read_len = fread(buffer, 1, buffer_size*2, file);
    buffer[read_len] = '\0';
    curr = buffer;
}

void inc_buffer()
{
    assert(read_len > buffer_size);
    read_len -= buffer_size;
    curr -= buffer_size;
    buffer_start += buffer_size;

    if(read_len < buffer_size)
    {
        assert(feof(file));
        return;
    }

    assert(read_len == buffer_size);
    memcpy(buffer, buffer+buffer_size, read_len);

    read_len += fread(buffer+buffer_size, 1, buffer_size, file);
    buffer[read_len] = '\0';

}

ebool inc_curr()
{
    curr++;
    if((curr - buffer) > buffer_size)
        inc_buffer();
    return feof(file);
}

bool is_whitespace(char c)
{
    return (c == ' ' ||
            c == '\t');
}

ebool consume_whitespace()
{
    while(is_whitespace(*curr) && !inc_curr());
    return feof(file);
}

const char* month_codes[] =
{
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
};

unsigned int num_months = 12;

int parse_month(const char* str)
{
    for(unsigned int i=0; i< num_months; i++)
    {
        if(strncmp(str, month_codes[i], 3) == 0)
            return i+1;
    }
    return -1;
}

bool is_num(char c)
{
    return (c >= '0' && c <= '9');
}

int parse_int()
{
    size_t max_int_size = 2; // Big enough for days, hours, mins, and seconds
    char int_buffer[max_int_size + 1];
    int i=0;

    while(i<max_int_size)
    {
        if(!is_num(*curr))
        {
            if(i == 0)
                return -1;
            else break;
        }
        int_buffer[i] = *curr;
        if(inc_curr())
            return -1;
        i++;
    }

    int_buffer[i] = '\0';

    return atoi(int_buffer);
}

ebool parse_char(char c)
{
    if(*curr == c)
        return inc_curr();
    else
        return false;
}

int find_next_date(off_t* out_offset = NULL, int* out_day = NULL)
{
    // Let's use pretty strict date parsing, don't want to run into something else by accident.
    // Could check for beginning of line, But really it's your own goddamn fault if your logs
    // have extra dates.

    while(!feof(file))
    {
        if(parse_month(curr) > 0)
        {
            off_t start_offset = buffer_start +
                (curr-buffer);

            if(inc_curr() || inc_curr() || inc_curr()) // Bleh!
                return -1;

            if(consume_whitespace())
                return -1;

            int day = parse_int();
            if(day == -1 || consume_whitespace())
                return -1;

            int hour = parse_int();
            if(hour == -1 || parse_char(':'))
                return -1;

            int minute = parse_int();
            if(minute == -1 || parse_char(':'))
                return -1;

            int second = parse_int();
            if(second == -1)
                return -1;

            printf("date(%td): %d %d:%d:%d\n", (ptrdiff_t)start_offset, day, hour, minute, second);

            int seconds = (hour * 60 * 60) +
                          (minute * 60) +
                          (second);

            // Be smarter about this, probally shouldn't assume that every other day is just the next, but it works for our needs.
            if(day != start_day)
                seconds += 24 * 60 * 60;

            if(out_offset)
                *out_offset = start_offset;

            if(out_day)
                *out_day = day;

            return seconds;
        }
        inc_curr();

    }
    return -1;
}

// In case we get the same time again, we will treat the timestamp as the first
// logical instance of that time, this gives us more useful results then trying
// both times and mixing up our results, It's unlikely that total number will be
// useful if we start counting two days. So: when we read in the time, normalize
// it to the first instance of time, the second time should always be after that.

int main(int argc, char** argv)
{
    const char* logfile = "data/small";
    file  = fopen(logfile, "rb");

    fill_buffer();

    find_next_date();

    fclose(file);
}
