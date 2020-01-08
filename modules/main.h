#ifndef _MAIN_H_
#define _MAIN_H_

#define MAIN(AppType) \
    int main(int argc, char** argv) { \
        AppType app; \
        app.Loop(); \
        return 0; \
    }

#endif // _MAIN_H_