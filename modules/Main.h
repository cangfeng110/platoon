#ifndef _TJP_MAIN_H_
#define _TJP_MAIN_H_

#define MAIN(AppType) \
    int main(int argc, char** argv) { \
        AppType app; \
        app.loop(); \
        return 0; \
    }

#endif // _TJP_MAIN_H_
