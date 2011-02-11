#include <cstdio>

// These are BIG log files! Thankfully you remembered to compile me 64-bit right?

FILE* file;
const size_t buffer_size = 10; // Small for testing purposes, enlarge for speed.
char buffer[buffer_size];
char* curr;
off_t buffer_start;

void fill_buffer()
{
    buffer_start = ftello(file);
    size_t read_len = fread(buffer, 1, buffer_size-1, file);
    buffer[read_len] = '\0';
    curr = buffer;
}

bool inc_curr()
{
    curr++;
    if(curr == '\0')
        fill_buffer();
    return feof(file);
}

void find_next_date()
{
    
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

    fclose(file);
}
