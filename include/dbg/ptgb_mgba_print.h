#ifndef _PTGB_MGBA_PRINT_H_
#define _PTGB_MGBA_PRINT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PTGB_MGBA_LOG_FATAL 0
#define PTGB_MGBA_LOG_ERROR 1
#define PTGB_MGBA_LOG_WARN 2
#define PTGB_MGBA_LOG_INFO 3
#define PTGB_MGBA_LOG_DEBUG 4

#define PTGB_MGBA_FATAL(...) ptgb_mgba_print(PTGB_MGBA_LOG_FATAL, __VA_ARGS__)
#define PTGB_MGBA_ERROR(...) ptgb_mgba_print(PTGB_MGBA_LOG_ERROR, __VA_ARGS__)
#define PTGB_MGBA_WARN(...) ptgb_mgba_print(PTGB_MGBA_LOG_WARN, __VA_ARGS__)
#define PTGB_MGBA_INFO(...) ptgb_mgba_print(PTGB_MGBA_LOG_INFO, __VA_ARGS__)
#define PTGB_MGBA_DEBUG(...) ptgb_mgba_print(PTGB_MGBA_LOG_DEBUG, __VA_ARGS__)

void ptgb_mgba_init(void);
void ptgb_mgba_deinit(void);

void ptgb_mgba_print(int level, const char *format_str, ...);

#ifdef __cplusplus
}
#endif

#endif