#include <cstdio>
#include <cstring>
#include <assert.h>

// These are BIG log files! Thankfully you remembered to compile me 64-bit right?

FILE* file;
const size_t buffer_size = 10; // Small for testing purposes, enlarge for speed.
char buffer[buffer_size*2+1];
size_t read_len;
char* curr;
off_t buffer_start;

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

bool inc_curr()
{
    curr++;
    if(curr == '\0')
        inc_buffer();
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

void find_next_date()
{
    // Let's use pretty strict date parsing, don't want to run into something else by accident.
    // Could check for beginning of line, But really it's your own goddamn fault if your logs
    // have extra dates.

    
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
