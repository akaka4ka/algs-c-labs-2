#ifndef CODE
#define CODE
#define WINSIZE 600
#define LINELENGTH 100

typedef struct LINE_SYS {
    int xPos;
    int yPos;
    int length;
    int velocity;
    int dx;
    int dy;
} LINE;
extern LINE line = { WINSIZE / 2 - LINELENGTH * 1.4 / 4, WINSIZE / 2 + LINELENGTH * 1.4 / 4, LINELENGTH * 1.4 / 2, 5, 0, 0 };

typedef struct TRACE_SYS {
    int* xTracePos;
    int* yTracePos;
    int traceLength;
} TRACE;
extern TRACE trace = { NULL, NULL, 5 };

void Code()
{
	trace.xTracePos = (int*)calloc(trace.traceLength, sizeof(int));
	trace.yTracePos = (int*)calloc(trace.traceLength, sizeof(int));
}

void TraceCalculation()
{
    int i = 0;
    for (i = trace.traceLength - 1; i > 0; i--)
    {
        trace.xTracePos[i] = trace.xTracePos[i - 1];
        trace.yTracePos[i] = trace.yTracePos[i - 1];
    }

    trace.xTracePos[i] = line.xPos;
    trace.yTracePos[i] = line.yPos;
}

#endif