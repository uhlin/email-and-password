#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"
#include "main.h"

#define SEPARATOR ','

static char database[_MAX_PATH]      = "";
static char email_file[_MAX_PATH]    = "";
static char password_file[_MAX_PATH] = "";

static FILE *database_fp      = NULL;
static FILE *email_file_fp    = NULL;
static FILE *password_file_fp = NULL;

__dead void
fatal(const char *text)
{
    if (database_fp)
	fclose(database_fp);
    if (email_file_fp)
	fclose(email_file_fp);
    if (password_file_fp)
	fclose(password_file_fp);

    MessageBox(NULL, text, "A fatal error occurred!", MB_OK | MB_ICONERROR);

    exit(EXIT_FAILURE);
}

static void
output_startup_notice()
{
    printf("%sEmail and Password%s v%s\n", BOLDWHITE, NORMAL, PROGVER);
    printf("A program made for Jeffster\n");
    printf("Compiled on %s %s\n", __DATE__, __TIME__);
    printf("%s----------------------------------------%s\n", GREEN, NORMAL);
}

static void
get_which_database()
{
    int c = EOF;

    while (true) {
	fputs("Which database?\nEnter filename: ", stdout);
	fflush(stdout);

	ZeroMemory(database, _countof(database));

	if (fgets(database, _countof(database), stdin) == NULL) {
	    puts("get_which_database: fgets() == NULL");
	} else if (strchr(database, '\n') == NULL) {
	    puts("get_which_database: input too big");

	    while (c = getchar(), c != '\n' && c != EOF)
		/* discard */;
	} else {
	    database[strcspn(database, "\n")] = '\0';

	    if (strings_match(database, ""))
		continue;

	    puts("OK");
	    break;
	}
    }
}

static void
get_which_email_file()
{
    int c = EOF;

    while (true) {
	fputs("Save to which email file?\nEnter filename: ", stdout);
	fflush(stdout);

	ZeroMemory(email_file, _countof(email_file));

	if (fgets(email_file, _countof(email_file), stdin) == NULL) {
	    puts("get_which_email_file: fgets() == NULL");
	} else if (strchr(email_file, '\n') == NULL) {
	    puts("get_which_email_file: input too big");

	    while (c = getchar(), c != '\n' && c != EOF)
		/* discard */;
	} else {
	    email_file[strcspn(email_file, "\n")] = '\0';

	    if (strings_match(email_file, ""))
		continue;

	    puts("OK");
	    break;
	}
    }
}

static void
get_which_password_file()
{
    int c = EOF;

    while (true) {
	fputs("Save to which password file?\nEnter filename: ", stdout);
	fflush(stdout);

	ZeroMemory(password_file, _countof(password_file));

	if (fgets(password_file, _countof(password_file), stdin) == NULL) {
	    puts("get_which_password_file: fgets() == NULL");
	} else if (strchr(password_file, '\n') == NULL) {
	    puts("get_which_password_file: input too big");

	    while (c = getchar(), c != '\n' && c != EOF)
		/* discard */;
	} else {
	    password_file[strcspn(password_file, "\n")] = '\0';

	    if (strings_match(password_file, ""))
		continue;

	    puts("OK");
	    break;
	}
    }
}

static void
output_report(const char *text)
{
    MessageBox(NULL, text, "Report", MB_OK | MB_ICONINFORMATION);
}

int
main()
{
    char message[1024] = "";

#ifdef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    VirtualTerminalProcessing();
#else
#pragma message("VirtualTerminalProcessing() unavailable!")
#endif
    output_startup_notice();

    get_which_database();
    if ((errno = fopen_s(&database_fp, database, "r")) != 0) {
	snprintf(message, _countof(message),
		 "Unable to open database: %s: %s",
		 database, strerror(errno));
	fatal(message);
    }

    get_which_email_file();
    if ((errno = fopen_s(&email_file_fp, email_file, "a")) != 0) {
	snprintf(message, _countof(message),
		 "Cannot open email file for appending: %s: %s",
		 email_file, strerror(errno));
	fatal(message);
    }

    get_which_password_file();
    if ((errno = fopen_s(&password_file_fp, password_file, "a")) != 0) {
	snprintf(message, _countof(message),
		 "Cannot open password file for appending: %s: %s",
		 password_file, strerror(errno));
	fatal(message);
    }

    char buf[8192] = { '\0' };
    long int line_no = 0;

    while (ZeroMemory(buf, _countof(buf)),
	   fgets(buf, _countof(buf), database_fp) != NULL) {
	char *cp = NULL;

	line_no ++;

	if ((cp = strchr(buf, SEPARATOR)) == NULL) {
	    snprintf(message, _countof(message),
		     "No separator found at line %ld", line_no);
	    fatal(message);
	}

	*cp = '\n';

	const char *token1 = strtok(buf, "\n");
	const char *token2 = strtok(NULL, "\n");

	if (token2 == NULL) {
	    fprintf(email_file_fp, "%s\n", "(null)");
	    fprintf(password_file_fp, "%s\n", token1);
	} else {
	    fprintf(email_file_fp, "%s\n", token1);
	    fprintf(password_file_fp, "%s\n", token2);
	}
    }

    if (feof(database_fp)) {
	/*
	 * Good
	 */
	fclose(database_fp);
	fclose(email_file_fp);
	fclose(password_file_fp);
    } else if (ferror(database_fp)) {
	fatal("main: fgets() returned null and the error indicator is set");
    } else {
	fatal("main: fgets() returned null for an unknown reason");
    }

    snprintf(message, _countof(message), "Done! %ld written items.", line_no);
    output_report(message);

    return EXIT_SUCCESS;
}
