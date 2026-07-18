/*
 * $LynxId: LYMessages_en.h,v 1.82 2019/08/24 00:37:02 tom Exp $
 *
 * Lynx - Hypertext navigation system
 *
 *   Copyright 1997-2019 Thomas E. Dickey
 *   Copyright 1992, 1993, 1994 University of Kansas
 *   Released 1995 using GNU General Public License
 */

#ifndef LYMESSAGES_EN_H
#define LYMESSAGES_EN_H

/*******************************************************************
 * The following definitions are for status line prompts, messages, or
 * warnings issued by Lynx during program execution.  You can modify
 * them to make them more appropriate for your site.  We recommend that
 * you extend these definitions to other languages using the gettext
 * library.  There are also scattered uses of 'gettext()' throughout the
 * Lynx source, covering all but those messages which (a) are used for
 * debugging (CTRACE) or (b) are constants used in interaction with
 * other programs.
 *
 * See ABOUT-NLS and po/readme for details and location of contributed
 * translations.  When no translation is available, the English default is
 * used.
 */
char *ALERT_FORMAT = gettext("Alert!: %s");
char *WELCOME_MSG = gettext("Welcome");
char *REALLY_QUIT = gettext("Are you sure you want to quit?");
#ifdef VMS
char *REALLY_EXIT = gettext("Really exit from Lynx?");
#endif /* VMS */
char *CONNECTION_INTERRUPTED = gettext("Connection interrupted.");
char *TRANSFER_INTERRUPTED = gettext("Data transfer interrupted.");
char *CANCELLED = gettext("Cancelled!!!");
char *CANCELLING = gettext("Cancelling!");
char *NO_CANCEL = gettext("Excellent!!!");
char *OPERATION_OK = gettext("OK");
char *OPERATION_DONE = gettext("Done!");
char *BAD_REQUEST = gettext("Bad request!");
char *PREVIOUS = gettext("previous");
char *NEXT_SCREEN = gettext("next screen");
char *TO_HELP = gettext("HELP!");
char *HELP_ON_SEGMENT = gettext(", help on ");
/* #define HELP */char *HELP = \
 gettext("Commands: Use arrow keys to move, '?' for help, 'q' to quit, '<-' to go back.");
/* #define MOREHELP */char *MOREHELP = \
 gettext("-- press space for more, use arrow keys to move, '?' for help, 'q' to quit.");
char *MORE = gettext("-- press space for next page --");
char *URL_TOO_LONG = gettext("URL too long");

/* Forms messages */
#ifdef TEXTFIELDS_MAY_NEED_ACTIVATION
/* Inactive input fields, messages used with -tna option - kw */
/* #define FORM_LINK_TEXT_ADV_MSG_INA */char *FORM_LINK_TEXT_ADV_MSG_INA = \
 gettext("(Textfield \"%s\"); Inactive.  Press <return> to activate.")
/* #define FORM_LINK_TEXT_MESSAGE_INA */char *FORM_LINK_TEXT_MESSAGE_INA = \
 gettext("(Text entry field) Inactive.  Press <return> to activate.");
/* #define FORM_LINK_TEXTAREA_ADV_MSG_INA */char *FORM_LINK_TEXTAREA_ADV_MSG_INA = \
 gettext("(Textarea \"%s\"); Inactive.  Press <return> to activate.")
/* #define FORM_LINK_TEXTAREA_MESSAGE_INA */char *FORM_LINK_TEXTAREA_MESSAGE_INA = \
 gettext("(Textarea) Inactive.  Press <return> to activate.");
/* #define FORM_LINK_TEXTAREA_ADV_MSG_INA_E */char *FORM_LINK_TEXTAREA_ADV_MSG_INA_E = \
 gettext("(Textarea \"%s\"); Inactive.  Press <return> to activate (%s for editor).")
/* #define FORM_LINK_TEXTAREA_MESSAGE_INA_E */char *FORM_LINK_TEXTAREA_MESSAGE_INA_E = \
 gettext("(Textarea) Inactive.  Press <return> to activate (%s for editor).");
/* #define FORM_LINK_TEXT_SUBMIT_MESSAGE_INA */char *FORM_LINK_TEXT_SUBMIT_MESSAGE_INA = \
 gettext("(Form field) Inactive.  Use <return> to edit.");
/* #define FORM_TEXT_SUBMIT_MESSAGE_INA_X */char *FORM_TEXT_SUBMIT_MESSAGE_INA_X = \
 gettext("(Form field) Inactive.  Use <return> to edit (%s to submit with no cache).");
/* #define FORM_TEXT_RESUBMIT_MESSAGE_INA */char *FORM_TEXT_RESUBMIT_MESSAGE_INA = \
 gettext("(Form field) Inactive. Press <return> to edit, press <return> twice to submit.");
/* #define FORM_TEXT_SUBMIT_MAILTO_MSG_INA */char *FORM_TEXT_SUBMIT_MAILTO_MSG_INA = \
 gettext("(mailto form field) Inactive.  Press <return> to change.");
/* #define FORM_LINK_PASSWORD_MESSAGE_INA */char *FORM_LINK_PASSWORD_MESSAGE_INA = \
 gettext("(Password entry field) Inactive.  Press <return> to activate.");
#endif
/* #define FORM_LINK_FILE_UNM_MSG */char *FORM_LINK_FILE_UNM_MSG = \
 gettext("UNMODIFIABLE file entry field.  Use UP or DOWN arrows or tab to move off.");
/* #define FORM_LINK_FILE_MESSAGE */char *FORM_LINK_FILE_MESSAGE = \
 gettext("(File entry field) Enter filename.  Use UP or DOWN arrows or tab to move off.");
/* #define FORM_LINK_TEXT_ADV_MSG */char *FORM_LINK_TEXT_ADV_MSG = \
 gettext("(Textfield \"%s\"); Enter text.  Use UP or DOWN arrows or tab to move off.")
/* #define FORM_LINK_TEXT_MESSAGE */char *FORM_LINK_TEXT_MESSAGE = \
 gettext("(Textfield \"%s\"); Enter text.  Use UP or DOWN arrows or tab to move off.")
/* #define FORM_LINK_TEXTAREA_ADV_MSG */char *FORM_LINK_TEXTAREA_ADV_MSG = \
 gettext("(Textarea \"%s\"); Enter text. Use UP/DOWN arrows or TAB to move off.")
/* #define FORM_LINK_TEXTAREA_MESSAGE */char *FORM_LINK_TEXTAREA_MESSAGE = \
 gettext("(Textarea) Enter text. Use UP/DOWN arrows or TAB to move off.");
/* #define FORM_LINK_TEXTAREA_ADV_MSG_E */char *FORM_LINK_TEXTAREA_ADV_MSG_E = \
 gettext("(Textarea \"%s\"); Enter text. Use UP/DOWN arrows or TAB to move off (%s for editor).")
/* #define FORM_LINK_TEXTAREA_MESSAGE_E */char *FORM_LINK_TEXTAREA_MESSAGE_E = \
 gettext("(Textarea) Enter text. Use UP/DOWN arrows or TAB to move off (%s for editor).");
/* #define FORM_LINK_TEXT_UNM_MSG */char *FORM_LINK_TEXT_UNM_MSG = \
 gettext("UNMODIFIABLE form text field.  Use UP or DOWN arrows or tab to move off.");
/* #define FORM_LINK_TEXT_SUBMIT_MESSAGE */char *FORM_LINK_TEXT_SUBMIT_MESSAGE = \
 gettext("(Form field) Enter text.  Use <return> to submit.");
/* #define FORM_LINK_TEXT_SUBMIT_MESSAGE_X */char *FORM_LINK_TEXT_SUBMIT_MESSAGE_X = \
 gettext("(Form field) Enter text.  Use <return> to submit (%s for no cache).");
/* #define FORM_LINK_TEXT_RESUBMIT_MESSAGE */char *FORM_LINK_TEXT_RESUBMIT_MESSAGE = \
 gettext("(Form field) Enter text.  Use <return> to submit, arrows or tab to move off.");
/* #define FORM_LINK_TEXT_SUBMIT_UNM_MSG */char *FORM_LINK_TEXT_SUBMIT_UNM_MSG = \
 gettext("UNMODIFIABLE form field.  Use UP or DOWN arrows or tab to move off.");
/* #define FORM_LINK_TEXT_SUBMIT_MAILTO_MSG */char *FORM_LINK_TEXT_SUBMIT_MAILTO_MSG = \
 gettext("(mailto form field) Enter text.  Use <return> to submit, arrows to move off.");
/* #define FORM_LINK_TEXT_SUBMIT_MAILTO_DIS_MSG */char *FORM_LINK_TEXT_SUBMIT_MAILTO_DIS_MSG = \
 gettext("(mailto form field) Mail is disallowed so you cannot submit.");
/* #define FORM_LINK_PASSWORD_MESSAGE */char *FORM_LINK_PASSWORD_MESSAGE = \
 gettext("(Password entry field) Enter text.  Use UP or DOWN arrows or tab to move off.");
/* #define FORM_LINK_PASSWORD_UNM_MSG */char *FORM_LINK_PASSWORD_UNM_MSG = \
 gettext("UNMODIFIABLE form password.  Use UP or DOWN arrows or tab to move off.");
/* #define FORM_LINK_CHECKBOX_ADV_MSG */char *FORM_LINK_CHECKBOX_ADV_MSG = \
 gettext("(Checkbox \"%s\");   Use right-arrow or <return> to toggle.")
/* #define FORM_LINK_CHECKBOX_MESSAGE */char *FORM_LINK_CHECKBOX_MESSAGE = \
 gettext("(Checkbox Field)   Use right-arrow or <return> to toggle.");
/* #define FORM_LINK_CHECKBOX_UNM_MSG */char *FORM_LINK_CHECKBOX_UNM_MSG = \
 gettext("UNMODIFIABLE form checkbox.  Use UP or DOWN arrows or tab to move off.");
/* #define FORM_LINK_RADIO_ADV_MSG */char *FORM_LINK_RADIO_ADV_MSG = \
 gettext("(Radio Button \"%s\");   Use right-arrow or <return> to toggle.")
/* #define FORM_LINK_RADIO_MESSAGE */char *FORM_LINK_RADIO_MESSAGE = \
 gettext("(Radio Button)   Use right-arrow or <return> to toggle.");
/* #define FORM_LINK_RADIO_UNM_MSG */char *FORM_LINK_RADIO_UNM_MSG = \
 gettext("UNMODIFIABLE form radio button.  Use UP or DOWN arrows or tab to move off.");
/* #define FORM_LINK_SUBMIT_PREFIX */char *FORM_LINK_SUBMIT_PREFIX = \
 gettext("Submit ('x' for no cache) to ");
/* #define FORM_LINK_RESUBMIT_PREFIX */char *FORM_LINK_RESUBMIT_PREFIX = \
 gettext("Submit to ");
/* #define FORM_LINK_SUBMIT_MESSAGE */char *FORM_LINK_SUBMIT_MESSAGE = \
 gettext("(Form submit button) Use right-arrow or <return> to submit ('x' for no cache).");
/* #define FORM_LINK_RESUBMIT_MESSAGE */char *FORM_LINK_RESUBMIT_MESSAGE = \
 gettext("(Form submit button) Use right-arrow or <return> to submit.");
/* #define FORM_LINK_SUBMIT_DIS_MSG */char *FORM_LINK_SUBMIT_DIS_MSG = \
 gettext("DISABLED form submit button.  Use UP or DOWN arrows or tab to move off.");
/* #define FORM_LINK_SUBMIT_MAILTO_PREFIX */char *FORM_LINK_SUBMIT_MAILTO_PREFIX = \
 gettext("Submit mailto form to ");
/* #define FORM_LINK_SUBMIT_MAILTO_MSG */char *FORM_LINK_SUBMIT_MAILTO_MSG = \
 gettext("(mailto form submit button) Use right-arrow or <return> to submit.");
/* #define FORM_LINK_SUBMIT_MAILTO_DIS_MSG */char *FORM_LINK_SUBMIT_MAILTO_DIS_MSG = \
 gettext("(mailto form submit button) Mail is disallowed so you cannot submit.");
/* #define FORM_LINK_RESET_MESSAGE */char *FORM_LINK_RESET_MESSAGE = \
 gettext("(Form reset button)   Use right-arrow or <return> to reset form to defaults.");
/* #define FORM_LINK_RESET_DIS_MSG */char *FORM_LINK_RESET_DIS_MSG = \
 gettext("DISABLED form reset button.  Use UP or DOWN arrows or tab to move off.");
/* #define FORM_LINK_BUTTON_MESSAGE */char *FORM_LINK_BUTTON_MESSAGE = \
 gettext("(Script button)   Use UP or DOWN arrows or tab to move off.");
/* #define FORM_LINK_BUTTON_ADV_MSG */char *FORM_LINK_BUTTON_ADV_MSG = \
 gettext("(Script button \"%s\");   Use UP or DOWN arrows or tab to move off.")
/* #define FORM_LINK_BUTTON_DIS_MSG */char *FORM_LINK_BUTTON_DIS_MSG = \
 gettext("DISABLED Script button.  Use UP or DOWN arrows or tab to move off.");
/* #define FORM_LINK_OPTION_LIST_ADV_MSG */char *FORM_LINK_OPTION_LIST_ADV_MSG = \
 gettext("(Option list \"%s\"); Hit return to select option.")
/* #define FORM_LINK_OPTION_LIST_MESSAGE */char *FORM_LINK_OPTION_LIST_MESSAGE = \
 gettext("(Option list) Hit return and use arrow keys and return to select option.");
/* #define CHOICE_LIST_ADV_MSG */char *CHOICE_LIST_ADV_MSG = \
 gettext("(Choice entry \"%s\"); Use arrow keys and return to select option.")
/* #define CHOICE_LIST_MESSAGE */char *CHOICE_LIST_MESSAGE = \
 gettext("(Choice list) Hit return and use arrow keys and return to select option.");
/* #define MOUSE_CHOICE_MESSAGE */char *MOUSE_CHOICE_MESSAGE = \
 gettext("Left mouse button or return to select, arrow keys to scroll.");
/* #define FORM_LINK_OPTION_LIST_UNM_MSG */char *FORM_LINK_OPTION_LIST_UNM_MSG = \
 gettext("UNMODIFIABLE option list.  Use return or arrow keys to review or leave.");
/* #define CHOICE_LIST_UNM_MSG */char *CHOICE_LIST_UNM_MSG = \
 gettext("UNMODIFIABLE choice list.  Use return or arrow keys to review or leave.");
char *SUBMITTING_FORM = gettext("Submitting form...");
char *RESETTING_FORM = gettext("Resetting form...");
/* #define RELOADING_FORM */char *RELOADING_FORM = \
 gettext("Reloading document.  Any form entries will be lost!");
/* #define LINK_NOT_IN_FORM */char *LINK_NOT_IN_FORM = \
 gettext("The current link is not in a FORM");
char *CANNOT_TRANSCODE_FORM = gettext("Warning: Cannot transcode form data to charset %s!");

/* #define NORMAL_LINK_MESSAGE */char *NORMAL_LINK_MESSAGE = \
 gettext("(NORMAL LINK)   Use right-arrow or <return> to activate.");
char *LINK_NOT_FOUND = gettext("The resource requested is not available at this time.");
char *ENTER_LYNX_COMMAND = gettext("Enter Lynx keystroke command: ");
char *WWW_FIND_MESSAGE = gettext("Looking up ");
char *WWW_WAIT_MESSAGE = gettext("Getting %s");
char *WWW_SKIP_MESSAGE = gettext("Skipping %s");
char *WWW_USING_MESSAGE = gettext("Using %s");
char *WWW_ILLEGAL_URL_MESSAGE = gettext("Illegal URL: %s");
char *WWW_BAD_ADDR_MESSAGE = gettext("Badly formed address %s");
char *ADVANCED_URL_MESSAGE = gettext("URL: %s");
char *WWW_FAIL_MESSAGE = gettext("Unable to access WWW file!!!");
char *WWW_INDEX_MESSAGE = gettext("This is a searchable index.  Use %s to search.");
/* #define WWW_INDEX_MORE_MESSAGE */char *WWW_INDEX_MORE_MESSAGE = \
 gettext("--More--  This is a searchable index.  Use %s to search.");
char *BAD_LINK_NUM_ENTERED = gettext("You have entered an invalid link number.");
/* #define SOURCE_HELP */char *SOURCE_HELP = \
 gettext("Currently viewing document source.  Press '\\' to return to rendered version.");
/* #define NOVICE_LINE_ONE */char *NOVICE_LINE_ONE = \
 gettext("  Arrow keys: Up and Down to move.  Right to follow a link; Left to go back.  \n");
/* #define NOVICE_LINE_TWO */char *NOVICE_LINE_TWO = \
 gettext(" H)elp O)ptions P)rint G)o M)ain screen Q)uit /=search [delete]=history list \n");
/* #define NOVICE_LINE_TWO_A */char *NOVICE_LINE_TWO_A = \
 gettext("  O)ther cmds  H)elp  K)eymap  G)oto  P)rint  M)ain screen  o)ptions  Q)uit  \n");
/* #define NOVICE_LINE_TWO_B */char *NOVICE_LINE_TWO_B = \
 gettext("  O)ther cmds  B)ack  E)dit  D)ownload ^R)eload ^W)ipe screen  search doc: / \n");
/* #define NOVICE_LINE_TWO_C */char *NOVICE_LINE_TWO_C = \
 gettext("O)ther cmds  C)omment  History: <backspace>  Bookmarks: V)iew, A)dd, R)emove \n");
/* #define FORM_NOVICELINE_ONE */char *FORM_NOVICELINE_ONE = \
 gettext("            Enter text into the field by typing on the keyboard              ");
/* #define FORM_NOVICELINE_TWO */char *FORM_NOVICELINE_TWO = \
 gettext("    Ctrl-U to delete all text in field, [Backspace] to delete a character    ");
/* #define FORM_NOVICELINE_TWO_DELBL */char *FORM_NOVICELINE_TWO_DELBL = \
 gettext("      Ctrl-U to delete text in field, [Backspace] to delete a character    ");
/* #define FORM_NOVICELINE_TWO_VAR */char *FORM_NOVICELINE_TWO_VAR = \
 gettext("    %s to delete all text in field, [Backspace] to delete a character    ");
/* #define FORM_NOVICELINE_TWO_DELBL_VAR */char *FORM_NOVICELINE_TWO_DELBL_VAR = \
 gettext("      %s to delete text in field, [Backspace] to delete a character    ");

/* mailto */
char *BAD_FORM_MAILTO = gettext("Malformed mailto form submission!  Cancelled!");
char *MAILTO_SQUASH_CTL = gettext("Warning!  Control codes in mail address replaced by ?");
char *FORM_MAILTO_DISALLOWED = gettext("Mail disallowed!  Cannot submit.");
char *FORM_MAILTO_FAILED = gettext("Mailto form submission failed!");
char *FORM_MAILTO_CANCELLED = gettext("Mailto form submission Cancelled!!!");
char *SENDING_FORM_CONTENT = gettext("Sending form content...");
char *NO_ADDRESS_IN_MAILTO_URL = gettext("No email address is present in mailto URL!");
/* #define MAILTO_URL_TEMPOPEN_FAILED */char *MAILTO_URL_TEMPOPEN_FAILED = \
 gettext("Unable to open temporary file for mailto URL!");
/* #define INC_ORIG_MSG_PROMPT */char *INC_ORIG_MSG_PROMPT = \
 gettext("Do you wish to include the original message?");
/* #define INC_PREPARSED_MSG_PROMPT */char *INC_PREPARSED_MSG_PROMPT = \
 gettext("Do you wish to include the preparsed source?");
/* #define SPAWNING_EDITOR_FOR_MAIL */char *SPAWNING_EDITOR_FOR_MAIL = \
 gettext("Spawning your selected editor to edit mail message");
/* #define ERROR_SPAWNING_EDITOR */char *ERROR_SPAWNING_EDITOR = \
 gettext("Error spawning editor, check your editor definition in the options menu");
char *SEND_COMMENT_PROMPT = gettext("Send this comment?");
char *SEND_MESSAGE_PROMPT = gettext("Send this message?");
char *SENDING_YOUR_MSG = gettext("Sending your message...");
char *SENDING_COMMENT = gettext("Sending your comment:");

/* textarea */
char *NOT_IN_TEXTAREA_NOEDIT = gettext("Not in a TEXTAREA; cannot use external editor.");
char *NOT_IN_TEXTAREA = gettext("Not in a TEXTAREA; cannot use command.");

char *FILE_ACTIONS_DISALLOWED = gettext("file: ACTIONs are disallowed!");
/* #define FILE_SERVED_LINKS_DISALLOWED */char *FILE_SERVED_LINKS_DISALLOWED = \
 gettext("file: URLs via served links are disallowed!");
char *NOAUTH_TO_ACCESS_FILES = gettext("Access to local files denied.");
char *FILE_BOOKMARKS_DISALLOWED = gettext("file: URLs via bookmarks are disallowed!");
/* #define SPECIAL_VIA_EXTERNAL_DISALLOWED */char *SPECIAL_VIA_EXTERNAL_DISALLOWED = \
 gettext("This special URL is not allowed in external documents!");
char *RETURN_TO_LYNX = gettext("Press <return> to return to Lynx.");
#ifdef VMS
/* #define SPAWNING_MSG */char *SPAWNING_MSG = \
 gettext("Spawning DCL subprocess.  Use 'logout' to return to Lynx.\n");
#else
#ifdef DOSPATH
/* #define SPAWNING_MSG */char *SPAWNING_MSG = \
 gettext("Type EXIT to return to Lynx.\n");
#else /* UNIX */
/* #define SPAWNING_MSG */char *SPAWNING_MSG = \
 gettext("Spawning your default shell.  Use 'exit' to return to Lynx.\n");
#endif
#endif /* VMS */
char *SPAWNING_DISABLED = gettext("Spawning is currently disabled.");
char *DOWNLOAD_DISABLED = gettext("The 'd'ownload command is currently disabled.");
char *NO_DOWNLOAD_INPUT = gettext("You cannot download an input field.");
char *NO_DOWNLOAD_MAILTO_ACTION = gettext("Form has a mailto action!  Cannot download.");
char *NO_DOWNLOAD_MAILTO_LINK = gettext("You cannot download a mailto: link.");
char *NO_DOWNLOAD_COOKIES = gettext("You cannot download cookies.");
char *NO_DOWNLOAD_PRINT_OP = gettext("You cannot download a printing option.");
char *NO_DOWNLOAD_UPLOAD_OP = gettext("You cannot download an upload option.");
char *NO_DOWNLOAD_PERMIT_OP = gettext("You cannot download an permit option.");
char *NO_DOWNLOAD_SPECIAL = gettext("This special URL cannot be downloaded!");
char *NO_DOWNLOAD_CHOICE = gettext("Nothing to download.");
char *TRACE_ON = gettext("Trace ON!");
char *TRACE_OFF = gettext("Trace OFF!");
/* #define CLICKABLE_IMAGES_ON */char *CLICKABLE_IMAGES_ON = \
 gettext("Links will be included for all images!  Reloading...");
/* #define CLICKABLE_IMAGES_OFF */char *CLICKABLE_IMAGES_OFF = \
 gettext("Standard image handling restored!  Reloading...");
/* #define PSEUDO_INLINE_ALTS_ON */char *PSEUDO_INLINE_ALTS_ON = \
 gettext("Pseudo_ALTs will be inserted for inlines without ALT strings!  Reloading...");
/* #define PSEUDO_INLINE_ALTS_OFF */char *PSEUDO_INLINE_ALTS_OFF = \
 gettext("Inlines without an ALT string specified will be ignored!  Reloading...");
char *RAWMODE_OFF = gettext("Raw 8-bit or CJK mode toggled OFF!  Reloading...");
char *RAWMODE_ON = gettext("Raw 8-bit or CJK mode toggled ON!  Reloading...");
/* #define HEAD_D_L_OR_CANCEL */char *HEAD_D_L_OR_CANCEL = \
 gettext("Send HEAD request for D)ocument or L)ink, or C)ancel? (d,l,c): ");
/* #define HEAD_D_OR_CANCEL */char *HEAD_D_OR_CANCEL = \
 gettext("Send HEAD request for D)ocument, or C)ancel? (d,c): ");
char *DOC_NOT_HTTP_URL = gettext("Sorry, the document is not an http URL.");
char *LINK_NOT_HTTP_URL = gettext("Sorry, the link is not an http URL.");
char *FORM_ACTION_DISABLED = gettext("Sorry, the ACTION for this form is disabled.");
/* #define FORM_ACTION_NOT_HTTP_URL */char *FORM_ACTION_NOT_HTTP_URL = \
 gettext("Sorry, the ACTION for this form is not an http URL.");
char *NOT_HTTP_URL_OR_ACTION = gettext("Not an http URL or form ACTION!");
char *SPECIAL_ACTION_DISALLOWED = gettext("This special URL cannot be a form ACTION!");
char *NOT_IN_STARTING_REALM = gettext("URL is not in starting realm!");
char *NEWSPOSTING_DISABLED = gettext("News posting is disabled!");
char *DIRED_DISABLED = gettext("File management support is disabled!");
char *NO_JUMPFILE = gettext("No jump file is currently available.");
char *JUMP_PROMPT = gettext("Jump to (use '?' for list): ");
char *JUMP_DISALLOWED = gettext("Jumping to a shortcut URL is disallowed!");
char *RANDOM_URL_DISALLOWED = gettext("Random URL is disallowed!  Use a shortcut.");
char *NO_RANDOM_URLS_YET = gettext("No random URLs have been used thus far.");
char *BOOKMARKS_DISABLED = gettext("Bookmark features are currently disabled.");
char *BOOKMARK_EXEC_DISABLED = gettext("Execution via bookmarks is disabled.");
/* #define BOOKMARK_FILE_NOT_DEFINED */char *BOOKMARK_FILE_NOT_DEFINED = \
 gettext("Bookmark file is not defined. Use %s to see options.");
/* #define NO_TEMP_FOR_HOTLIST */char *NO_TEMP_FOR_HOTLIST = \
 gettext("Unable to open tempfile for X Mosaic hotlist conversion.");
char *BOOKMARK_OPEN_FAILED = gettext("ERROR - unable to open bookmark file.");
/* #define BOOKMARK_OPEN_FAILED_FOR_DEL */char *BOOKMARK_OPEN_FAILED_FOR_DEL = \
 gettext("Unable to open bookmark file for deletion of link.");
/* #define BOOKSCRA_OPEN_FAILED_FOR_DEL */char *BOOKSCRA_OPEN_FAILED_FOR_DEL = \
 gettext("Unable to open scratch file for deletion of link.");
#ifdef VMS
char *ERROR_RENAMING_SCRA = gettext("Error renaming scratch file.");
#else
char *ERROR_RENAMING_TEMP = gettext("Error renaming temporary file.");
/* #define BOOKTEMP_COPY_FAIL */char *BOOKTEMP_COPY_FAIL = \
 gettext("Unable to copy temporary file for deletion of link.");
/* #define BOOKTEMP_REOPEN_FAIL_FOR_DEL */char *BOOKTEMP_REOPEN_FAIL_FOR_DEL = \
 gettext("Unable to reopen temporary file for deletion of link.");
#endif /* VMS */
/* #define BOOKMARK_LINK_NOT_ONE_LINE */char *BOOKMARK_LINK_NOT_ONE_LINE = \
 gettext("Link is not by itself all on one line in bookmark file.");
char *BOOKMARK_DEL_FAILED = gettext("Bookmark deletion failed.");
/* #define BOOKMARKS_NOT_TRAVERSED */char *BOOKMARKS_NOT_TRAVERSED = \
 gettext("Bookmark files cannot be traversed (only http URLs).");
/* #define BOOKMARKS_NOT_OPEN */char *BOOKMARKS_NOT_OPEN = \
 gettext("Unable to open bookmark file, use 'a' to save a link first");
char *BOOKMARKS_NOLINKS = gettext("There are no links in this bookmark file!");
/* #define CACHE_D_OR_CANCEL */char *CACHE_D_OR_CANCEL = \
 gettext("D)elete cached document or C)ancel? (d,c): ");
/* #define BOOK_D_L_OR_CANCEL */char *BOOK_D_L_OR_CANCEL = \
 gettext("Save D)ocument or L)ink to bookmark file or C)ancel? (d,l,c): ");
char *BOOK_D_OR_CANCEL = gettext("Save D)ocument to bookmark file or C)ancel? (d,c): ");
char *BOOK_L_OR_CANCEL = gettext("Save L)ink to bookmark file or C)ancel? (l,c): ");
/* #define NOBOOK_POST_FORM */char *NOBOOK_POST_FORM = \
 gettext("Documents from forms with POST content cannot be saved as bookmarks.");
char *NOBOOK_FORM_FIELD = gettext("Cannot save form fields/links");
/* #define NOBOOK_HSML */char *NOBOOK_HSML = \
 gettext("History, showinfo, menu and list files cannot be saved as bookmarks.");
/* #define CONFIRM_BOOKMARK_DELETE */char *CONFIRM_BOOKMARK_DELETE = \
 gettext("Do you really want to delete this link from your bookmark file?");
char *MALFORMED_ADDRESS = gettext("Malformed address.");
/* #define HISTORICAL_ON_MINIMAL_OFF */char *HISTORICAL_ON_MINIMAL_OFF = \
 gettext("Historical comment parsing ON (Minimal is overridden)!");
/* #define HISTORICAL_OFF_MINIMAL_ON */char *HISTORICAL_OFF_MINIMAL_ON = \
 gettext("Historical comment parsing OFF (Minimal is in effect)!");
/* #define HISTORICAL_ON_VALID_OFF */char *HISTORICAL_ON_VALID_OFF = \
 gettext("Historical comment parsing ON (Valid is overridden)!");
/* #define HISTORICAL_OFF_VALID_ON */char *HISTORICAL_OFF_VALID_ON = \
 gettext("Historical comment parsing OFF (Valid is in effect)!");
/* #define MINIMAL_ON_IN_EFFECT */char *MINIMAL_ON_IN_EFFECT = \
 gettext("Minimal comment parsing ON (and in effect)!");
/* #define MINIMAL_OFF_VALID_ON */char *MINIMAL_OFF_VALID_ON = \
 gettext("Minimal comment parsing OFF (Valid is in effect)!");
/* #define MINIMAL_ON_BUT_HISTORICAL */char *MINIMAL_ON_BUT_HISTORICAL = \
 gettext("Minimal comment parsing ON (but Historical is in effect)!");
/* #define MINIMAL_OFF_HISTORICAL_ON */char *MINIMAL_OFF_HISTORICAL_ON = \
 gettext("Minimal comment parsing OFF (Historical is in effect)!");
char *SOFT_DOUBLE_QUOTE_ON = gettext("Soft double-quote parsing ON!");
char *SOFT_DOUBLE_QUOTE_OFF = gettext("Soft double-quote parsing OFF!");
char *USING_DTD_0 = gettext("Now using TagSoup parsing of HTML.");
char *USING_DTD_1 = gettext("Now using SortaSGML parsing of HTML!");
char *ALREADY_AT_END = gettext("You are already at the end of this document.");
char *ALREADY_AT_BEGIN = gettext("You are already at the beginning of this document.");
char *ALREADY_AT_PAGE = gettext("You are already at page %d of this document.");
char *LINK_ALREADY_CURRENT = gettext("Link number %d already is current.");
char *ALREADY_AT_FIRST = gettext("You are already at the first document");
char *NO_LINKS_ABOVE = gettext("There are no links above this line of the document.");
char *NO_LINKS_BELOW = gettext("There are no links below this line of the document.");
/* #define MAXLEN_REACHED_DEL_OR_MOV */char *MAXLEN_REACHED_DEL_OR_MOV = \
 gettext("Maximum length reached!  Delete text or move off field.");
/* #define NOT_ON_SUBMIT_OR_LINK */char *NOT_ON_SUBMIT_OR_LINK = \
 gettext("You are not on a form submission button or normal link.");
/* #define NEED_CHECKED_RADIO_BUTTON */char *NEED_CHECKED_RADIO_BUTTON = \
 gettext("One radio button must be checked at all times!");
char *NO_SUBMIT_BUTTON_QUERY = gettext("No submit button for this form, submit single text field?");
char *PREV_DOC_QUERY = gettext("Do you want to go back to the previous document?");
char *ARROWS_OR_TAB_TO_MOVE = gettext("Use arrows or tab to move off of field.");
/* #define ENTER_TEXT_ARROWS_OR_TAB */char *ENTER_TEXT_ARROWS_OR_TAB = \
 gettext("Enter text.  Use arrows or tab to move off of field.");
char *NO_FORM_ACTION = gettext("** Bad HTML!!  No form action defined. **");
char *BAD_HTML_NO_POPUP = gettext("Bad HTML!!  Unable to create popup window!");
char *POPUP_FAILED = gettext("Unable to create popup window!");
char *GOTO_DISALLOWED = gettext("Goto a random URL is disallowed!");
char *GOTO_NON_HTTP_DISALLOWED = gettext("Goto a non-http URL is disallowed!");
char *GOTO_XXXX_DISALLOWED = gettext("You are not allowed to goto \"%s\" URLs");
char *URL_TO_OPEN = gettext("URL to open: ");
char *EDIT_CURRENT_GOTO = gettext("Edit the current Goto URL: ");
char *EDIT_THE_PREV_GOTO = gettext("Edit the previous Goto URL: ");
char *EDIT_A_PREV_GOTO = gettext("Edit a previous Goto URL: ");
char *CURRENT_DOC_HAS_POST_DATA = gettext("Current document has POST data.");
char *EDIT_CURDOC_URL = gettext("Edit this document's URL: ");
char *EDIT_CURLINK_URL = gettext("Edit the current link's URL: ");
char *EDIT_SUBMIT_URL = gettext("Edit the form's submit-URL: ");
char *EDIT_FM_MENU_URLS_DISALLOWED = gettext("You cannot edit File Management URLs");
char *ENTER_DATABASE_QUERY = gettext("Enter a database query: ");
char *ENTER_WHEREIS_QUERY = gettext("Enter a whereis query: ");
char *EDIT_CURRENT_QUERY = gettext("Edit the current query: ");
char *EDIT_THE_PREV_QUERY = gettext("Edit the previous query: ");
char *EDIT_A_PREV_QUERY = gettext("Edit a previous query: ");
/* #define USE_C_R_TO_RESUB_CUR_QUERY */char *USE_C_R_TO_RESUB_CUR_QUERY = \
 gettext("Use Control-R to resubmit the current query.");
char *EDIT_CURRENT_SHORTCUT = gettext("Edit the current shortcut: ");
char *EDIT_THE_PREV_SHORTCUT = gettext("Edit the previous shortcut: ");
char *EDIT_A_PREV_SHORTCUT = gettext("Edit a previous shortcut: ");
char *KEY_NOT_MAPPED_TO_JUMP_FILE = gettext("Key '%c' is not mapped to a jump file!");
char *CANNOT_LOCATE_JUMP_FILE = gettext("Cannot locate jump file!");
char *CANNOT_OPEN_JUMP_FILE = gettext("Cannot open jump file!");
char *ERROR_READING_JUMP_FILE = gettext("Error reading jump file!");
char *OUTOF_MEM_FOR_JUMP_FILE = gettext("Out of memory reading jump file!");
char *OUTOF_MEM_FOR_JUMP_TABLE = gettext("Out of memory reading jump table!");
char *NO_INDEX_FILE = gettext("No index is currently available.");
/* #define CONFIRM_MAIN_SCREEN */char *CONFIRM_MAIN_SCREEN = \
 gettext("Do you really want to go to the Main screen?");
char *IN_MAIN_SCREEN = gettext("You are already at main screen!");
/* #define NOT_ISINDEX */char *NOT_ISINDEX = \
 gettext("Not a searchable indexed document -- press '/' to search for a text string");
/* #define NO_OWNER */char *NO_OWNER = \
 gettext("No owner is defined for this file so you cannot send a comment");
char *NO_OWNER_USE = gettext("No owner is defined. Use %s?");
char *CONFIRM_COMMENT = gettext("Do you wish to send a comment?");
char *MAIL_DISALLOWED = gettext("Mail is disallowed so you cannot send a comment");
char *EDIT_DISABLED = gettext("The 'e'dit command is currently disabled.");
char *ANYEDIT_DISABLED = gettext("External editing is currently disabled.");
char *NO_STATUS = gettext("System error - failure to get status.");
char *NO_EDITOR = gettext("No editor is defined!");
char *PRINT_DISABLED = gettext("The 'p'rint command is currently disabled.");
char *NO_TOOLBAR = gettext("Document has no Toolbar links or Banner.");
char *CANNOT_OPEN_TRAV_FILE = gettext("Unable to open traversal file.");
char *CANNOT_OPEN_TRAF_FILE = gettext("Unable to open traversal found file.");
char *CANNOT_OPEN_REJ_FILE = gettext("Unable to open reject file.");
char *NOOPEN_TRAV_ERR_FILE = gettext("Unable to open traversal errors output file");
char *TRAV_WAS_INTERRUPTED = gettext("TRAVERSAL WAS INTERRUPTED");
char *FOLLOW_LINK_NUMBER = gettext("Follow link (or goto link or page) number: ");
char *SELECT_OPTION_NUMBER = gettext("Select option (or page) number: ");
char *OPTION_ALREADY_CURRENT = gettext("Option number %d already is current.");
/* #define ALREADY_AT_OPTION_END */char *ALREADY_AT_OPTION_END = \
 gettext("You are already at the end of this option list.");
/* #define ALREADY_AT_OPTION_BEGIN */char *ALREADY_AT_OPTION_BEGIN = \
 gettext("You are already at the beginning of this option list.");
/* #define ALREADY_AT_OPTION_PAGE */char *ALREADY_AT_OPTION_PAGE = \
 gettext("You are already at page %d of this option list.");
char *BAD_OPTION_NUM_ENTERED = gettext("You have entered an invalid option number.");
char *BAD_HTML_USE_TRACE = gettext("** Bad HTML!!  Use -trace to diagnose. **");
char *GIVE_FILENAME = gettext("Give name of file to save in");
char *CANNOT_SAVE_REMOTE = gettext("Can't save data to file -- please run WWW locally");
char *CANNOT_OPEN_TEMP = gettext("Can't open temporary file!");
char *CANNOT_OPEN_OUTPUT = gettext("Can't open output file!  Cancelling!");
char *EXECUTION_DISABLED = gettext("Execution is disabled.");
/* #define EXECUTION_DISABLED_FOR_FILE */char *EXECUTION_DISABLED_FOR_FILE = \
 gettext("Execution is not enabled for this file.  See the Options menu (use %s).");
/* #define EXECUTION_NOT_COMPILED */char *EXECUTION_NOT_COMPILED = \
 gettext("Execution capabilities are not compiled into this version.");
char *CANNOT_DISPLAY_FILE = gettext("This file cannot be displayed on this terminal.");
/* #define CANNOT_DISPLAY_FILE_D_OR_C */char *CANNOT_DISPLAY_FILE_D_OR_C = \
 gettext("This file cannot be displayed on this terminal:  D)ownload, or C)ancel");
char *MSG_DOWNLOAD_OR_CANCEL = gettext("%s  D)ownload, or C)ancel");
char *CANCELLING_FILE = gettext("Cancelling file.");
char *RETRIEVING_FILE = gettext("Retrieving file.  - PLEASE WAIT -");
char *FILENAME_PROMPT = gettext("Enter a filename: ");
char *EDIT_THE_PREV_FILENAME = gettext("Edit the previous filename: ");
char *EDIT_A_PREV_FILENAME = gettext("Edit a previous filename: ");
char *NEW_FILENAME_PROMPT = gettext("Enter a new filename: ");
char *FILENAME_CANNOT_BE_DOT = gettext("File name may not begin with a dot.");
#ifdef VMS
char *FILE_EXISTS_HPROMPT = gettext("File exists.  Create higher version?");
#else
char *FILE_EXISTS_OPROMPT = gettext("File exists.  Overwrite?");
#endif /* VMS */
char *CANNOT_WRITE_TO_FILE = gettext("Cannot write to file.");
char *MISCONF_DOWNLOAD_COMMAND = gettext("ERROR! - download command is misconfigured.");
char *CANNOT_DOWNLOAD_FILE = gettext("Unable to download file.");
char *READING_DIRECTORY = gettext("Reading directory...");
char *BUILDING_DIR_LIST = gettext("Building directory listing...");
char *SAVING = gettext("Saving...");
char *COULD_NOT_EDIT_FILE = gettext("Could not edit file '%s'.");
char *COULD_NOT_ACCESS_DOCUMENT = gettext("Unable to access document!");
char *COULD_NOT_ACCESS_FILE = gettext("Could not access file.");
char *COULD_NOT_ACCESS_DIR = gettext("Could not access directory.");
char *COULD_NOT_LOAD_DATA = gettext("Could not load data.");
/* #define CANNOT_EDIT_REMOTE_FILES */char *CANNOT_EDIT_REMOTE_FILES = \
 gettext("Lynx cannot currently (e)dit remote WWW files.");
/* #define CANNOT_EDIT_FIELD */char *CANNOT_EDIT_FIELD = \
 gettext("This field cannot be (e)dited with an external editor.");
char *RULE_INCORRECT = gettext("Bad rule");
char *RULE_NEEDS_DATA = gettext("Insufficient operands:");
char *NOAUTH_TO_EDIT_FILE = gettext("You are not authorized to edit this file.");
char *TITLE_PROMPT = gettext("Title: ");
char *SUBJECT_PROMPT = gettext("Subject: ");
char *USERNAME_PROMPT = gettext("Username: ");
char *PASSWORD_PROMPT = gettext("Password: ");
char *USERNAME_PASSWORD_REQUIRED = gettext("lynx: Username and Password required!!!");
char *PASSWORD_REQUIRED = gettext("lynx: Password required!!!");
char *CLEAR_ALL_AUTH_INFO = gettext("Clear all authorization info for this session?");
char *AUTH_INFO_CLEARED = gettext("Authorization info cleared.");
char *AUTH_FAILED_PROMPT = gettext("Authorization failed.  Retry?");
char *CGI_DISABLED = gettext("cgi support has been disabled.");
/* #define CGI_NOT_COMPILED */char *CGI_NOT_COMPILED = \
 gettext("Lynxcgi capabilities are not compiled into this version.");
char *CANNOT_CONVERT_I_TO_O = gettext("Sorry, no known way of converting %s to %s.");
char *CONNECT_SET_FAILED = gettext("Unable to set up connection.");
char *CONNECT_FAILED = gettext("Unable to make connection");
/* #define MALFORMED_EXEC_REQUEST */char *MALFORMED_EXEC_REQUEST = \
 gettext("Executable link rejected due to malformed request.");
/* #define BADCHAR_IN_EXEC_LINK */char *BADCHAR_IN_EXEC_LINK = \
 gettext("Executable link rejected due to `%c' character.");
/* #define RELPATH_IN_EXEC_LINK */char *RELPATH_IN_EXEC_LINK = \
 gettext("Executable link rejected due to relative path string ('../').");
/* #define BADLOCPATH_IN_EXEC_LINK */char *BADLOCPATH_IN_EXEC_LINK = \
 gettext("Executable link rejected due to location or path.");
char *MAIL_DISABLED = gettext("Mail access is disabled!");
/* #define ACCESS_ONLY_LOCALHOST */char *ACCESS_ONLY_LOCALHOST = \
 gettext("Only files and servers on the local host can be accessed.");
char *TELNET_DISABLED = gettext("Telnet access is disabled!");
/* #define TELNET_PORT_SPECS_DISABLED */char *TELNET_PORT_SPECS_DISABLED = \
 gettext("Telnet port specifications are disabled.");
char *NEWS_DISABLED = gettext("USENET news access is disabled!");
char *RLOGIN_DISABLED = gettext("Rlogin access is disabled!");
char *FTP_DISABLED = gettext("Ftp access is disabled!");
char *NO_REFS_FROM_DOC = gettext("There are no references from this document.");
char *NO_VISIBLE_REFS_FROM_DOC = gettext("There are only hidden links from this document.");
#ifdef VMS
char *CANNOT_OPEN_COMFILE = gettext("Unable to open command file.");
#endif /* VMS */
char *NEWS_POST_CANCELLED = gettext("News Post Cancelled!!!");
/* #define SPAWNING_EDITOR_FOR_NEWS */char *SPAWNING_EDITOR_FOR_NEWS = \
 gettext("Spawning your selected editor to edit news message");
char *POST_MSG_PROMPT = gettext("Post this message?");
char *APPEND_SIG_FILE = gettext("Append '%s'?");
char *POSTING_TO_NEWS = gettext("Posting to newsgroup(s)...");
#ifdef VMS
char *HAVE_UNREAD_MAIL_MSG = gettext("*** You have unread mail. ***");
#else
char *HAVE_MAIL_MSG = gettext("*** You have mail. ***");
#endif /* VMS */
char *HAVE_NEW_MAIL_MSG = gettext("*** You have new mail. ***");
char *FILE_INSERT_CANCELLED = gettext("File insert cancelled!!!");
char *MEMORY_EXHAUSTED_FILE = gettext("Not enough memory for file!");
char *FILE_CANNOT_OPEN_R = gettext("Can't open file for reading.");
char *FILE_DOES_NOT_EXIST = gettext("File does not exist.");
char *FILE_DOES_NOT_EXIST_RE = gettext("File does not exist - reenter or cancel:");
char *FILE_NOT_READABLE = gettext("File is not readable.");
char *FILE_NOT_READABLE_RE = gettext("File is not readable - reenter or cancel:");
char *FILE_INSERT_0_LENGTH = gettext("Nothing to insert - file is 0-length.");
char *SAVE_REQUEST_CANCELLED = gettext("Save request cancelled!!!");
char *MAIL_REQUEST_CANCELLED = gettext("Mail request cancelled!!!");
/* #define CONFIRM_MAIL_SOURCE_PREPARSED */char *CONFIRM_MAIL_SOURCE_PREPARSED = \
 gettext("Viewing preparsed source.  Are you sure you want to mail it?");
char *PLEASE_WAIT = gettext("Please wait...");
char *MAILING_FILE = gettext("Mailing file.  Please wait...");
char *MAIL_REQUEST_FAILED = gettext("ERROR - Unable to mail file");
/* #define CONFIRM_LONG_SCREEN_PRINT */char *CONFIRM_LONG_SCREEN_PRINT = \
 gettext("File is %d screens long.  Are you sure you want to print?");
char *PRINT_REQUEST_CANCELLED = gettext("Print request cancelled!!!");
char *PRESS_RETURN_TO_BEGIN = gettext("Press <return> to begin: ");
char *PRESS_RETURN_TO_FINISH = gettext("Press <return> to finish: ");
/* #define CONFIRM_LONG_PAGE_PRINT */char *CONFIRM_LONG_PAGE_PRINT = \
 gettext("File is %d pages long.  Are you sure you want to print?");
/* #define CHECK_PRINTER */char *CHECK_PRINTER = \
 gettext("Be sure your printer is on-line.  Press <return> to start printing:");
char *FILE_ALLOC_FAILED = gettext("ERROR - Unable to allocate file space!!!");
char *UNABLE_TO_OPEN_TEMPFILE = gettext("Unable to open tempfile");
char *UNABLE_TO_OPEN_PRINTOP_FILE = gettext("Unable to open print options file");
char *PRINTING_FILE = gettext("Printing file.  Please wait...");
char *MAIL_ADDRESS_PROMPT = gettext("Please enter a valid internet mail address: ");
char *PRINTER_MISCONF_ERROR = gettext("ERROR! - printer is misconfigured!");
char *FAILED_MAP_POST_REQUEST = gettext("Image map from POST response not available!");
char *MISDIRECTED_MAP_REQUEST = gettext("Misdirected client-side image MAP request!");
char *MAP_NOT_ACCESSIBLE = gettext("Client-side image MAP is not accessible!");
char *MAPS_NOT_AVAILABLE = gettext("No client-side image MAPs are available!");
char *MAP_NOT_AVAILABLE = gettext("Client-side image MAP is not available!");
#ifndef NO_OPTION_MENU
/* #define OPTION_SCREEN_NEEDS_24 */char *OPTION_SCREEN_NEEDS_24 = \
 gettext("Screen height must be at least 24 lines for the Options menu!");
/* #define OPTION_SCREEN_NEEDS_23 */char *OPTION_SCREEN_NEEDS_23 = \
 gettext("Screen height must be at least 23 lines for the Options menu!");
/* #define OPTION_SCREEN_NEEDS_22 */char *OPTION_SCREEN_NEEDS_22 = \
 gettext("Screen height must be at least 22 lines for the Options menu!");
#endif /* !NO_OPTION_MENU */
char *NEED_ADVANCED_USER_MODE = gettext("That key requires Advanced User mode.");
char *CONTENT_TYPE_MSG = gettext("Content-type: %s");
char *COMMAND_PROMPT = gettext("Command: ");
char *COMMAND_UNKNOWN = gettext("Unknown or ambiguous command");
char *VERSION_SEGMENT = gettext(" Version ");
char *FIRST_SEGMENT = gettext(" first");
char *GUESSING_SEGMENT = gettext(", guessing...");
char *PERMISSIONS_SEGMENT = gettext("Permissions for ");
char *SELECT_SEGMENT = gettext("Select ");
char *CAP_LETT_SEGMENT = gettext("capital letter");
char *OF_OPT_LINE_SEGMENT = gettext(" of option line,");
char *TO_SAVE_SEGMENT = gettext(" to save,");
char *TO_SEGMENT = gettext(" to ");
char *OR_SEGMENT = gettext(" or ");
char *INDEX_SEGMENT = gettext(" index");
char *TO_RETURN_SEGMENT = gettext(" to return to Lynx.");
char *ACCEPT_CHANGES = gettext("Accept Changes");
char *RESET_CHANGES = gettext("Reset Changes");
char *CANCEL_CHANGES = gettext("Left Arrow cancels changes");
char *SAVE_OPTIONS = gettext("Save options to disk");
char *ACCEPT_DATA = gettext("Hit RETURN to accept entered data.");
/* #define ACCEPT_DATA_OR_DEFAULT */char *ACCEPT_DATA_OR_DEFAULT = \
 gettext("Hit RETURN to accept entered data.  Delete data to invoke the default.");
char *VALUE_ACCEPTED = gettext("Value accepted!");
/* #define VALUE_ACCEPTED_WARNING_X */char *VALUE_ACCEPTED_WARNING_X = \
 gettext("Value accepted! -- WARNING: Lynx is configured for XWINDOWS!");
/* #define VALUE_ACCEPTED_WARNING_NONX */char *VALUE_ACCEPTED_WARNING_NONX = \
 gettext("Value accepted! -- WARNING: Lynx is NOT configured for XWINDOWS!");
char *EDITOR_LOCKED = gettext("You are not allowed to change which editor to use!");
char *FAILED_TO_SET_DISPLAY = gettext("Failed to set DISPLAY variable!");
char *FAILED_CLEAR_SET_DISPLAY = gettext("Failed to clear DISPLAY variable!");
/* #define BOOKMARK_CHANGE_DISALLOWED */char *BOOKMARK_CHANGE_DISALLOWED = \
 gettext("You are not allowed to change the bookmark file!");
char *COLOR_TOGGLE_DISABLED = gettext("Terminal does not support color");
char *COLOR_TOGGLE_DISABLED_FOR_TERM = gettext("Your '%s' terminal does not support color.");
char *DOTFILE_ACCESS_DISABLED = gettext("Access to dot files is disabled!");
/* #define UA_NO_LYNX_WARNING */char *UA_NO_LYNX_WARNING = \
 gettext("User-Agent string does not contain \"Lynx\" or \"L_y_n_x\"");
/* #define UA_PLEASE_USE_LYNX */char *UA_PLEASE_USE_LYNX = \
 gettext("Use \"L_y_n_x\" or \"Lynx\" in User-Agent, or it looks like intentional deception!");
/* #define UA_CHANGE_DISABLED */char *UA_CHANGE_DISABLED = \
 gettext("Changing of the User-Agent string is disabled!");
/* #define CHANGE_OF_SETTING_DISALLOWED */char *CHANGE_OF_SETTING_DISALLOWED = \
 gettext("You are not allowed to change this setting.");
char *SAVING_OPTIONS = gettext("Saving Options...");
char *OPTIONS_SAVED = gettext("Options saved!");
char *OPTIONS_NOT_SAVED = gettext("Unable to save Options!");
char *R_TO_RETURN_TO_LYNX = gettext(" 'r' to return to Lynx ");
char *SAVE_OR_R_TO_RETURN_TO_LYNX = gettext(" '>' to save, or 'r' to return to Lynx ");
/* #define ANY_KEY_CHANGE_RET_ACCEPT */char *ANY_KEY_CHANGE_RET_ACCEPT = \
 gettext("Hit any key to change value; RETURN to accept.");
char *ERROR_UNCOMPRESSING_TEMP = gettext("Error uncompressing temporary file!");
char *UNSUPPORTED_URL_SCHEME = gettext("Unsupported URL scheme!");
char *UNSUPPORTED_DATA_URL = gettext("Unsupported data: URL!  Use SHOWINFO, for now.");
char *TOO_MANY_REDIRECTIONS = gettext("Redirection limit of %d URL's reached.");
char *ILLEGAL_REDIRECTION_URL = gettext("Illegal redirection URL received from server!");
/* #define SERVER_ASKED_FOR_REDIRECTION */char *SERVER_ASKED_FOR_REDIRECTION = \
 gettext("Server asked for %d redirection of POST content to");
#define REDIRECTION_WITH_BAD_LOCATION "Got redirection with a bad Location header."
#define REDIRECTION_WITH_NO_LOCATION "Got redirection with no Location header."
char *PROCEED_GET_CANCEL = gettext("P)roceed, use G)ET or C)ancel ");
char *PROCEED_OR_CANCEL = gettext("P)roceed, or C)ancel ");
/* #define ADVANCED_POST_GET_REDIRECT */char *ADVANCED_POST_GET_REDIRECT = \
 gettext("Redirection of POST content.  P)roceed, see U)RL, use G)ET or C)ancel");
/* #define ADVANCED_POST_REDIRECT */char *ADVANCED_POST_REDIRECT = \
 gettext("Redirection of POST content.  P)roceed, see U)RL, or C)ancel");
/* #define CONFIRM_POST_RESUBMISSION */char *CONFIRM_POST_RESUBMISSION = \
 gettext("Document from Form with POST content.  Resubmit?");
/* #define CONFIRM_POST_RESUBMISSION_TO */char *CONFIRM_POST_RESUBMISSION_TO = \
 gettext("Resubmit POST content to %s ?");
/* #define CONFIRM_POST_LIST_RELOAD */char *CONFIRM_POST_LIST_RELOAD = \
 gettext("List from document with POST data.  Reload %s ?");
/* #define CONFIRM_POST_DOC_HEAD */char *CONFIRM_POST_DOC_HEAD = \
 gettext("Document from POST action, HEAD may not be understood.  Proceed?");
/* #define CONFIRM_POST_LINK_HEAD */char *CONFIRM_POST_LINK_HEAD = \
 gettext("Form submit action is POST, HEAD may not be understood.  Proceed?");
char *CONFIRM_WO_PASSWORD = gettext("Proceed without a username and password?");
char *CONFIRM_PROCEED = gettext("Proceed (%s)?");
char *CANNOT_POST = gettext("Cannot POST to this host.");
char *IGNORED_POST = gettext("POST not supported for this URL - ignoring POST data!");
char *DISCARDING_POST_DATA = gettext("Discarding POST data...");
char *WILL_NOT_RELOAD_DOC = gettext("Document will not be reloaded!");
char *LOCATION_HEADER = gettext("Location: ");
char *STRING_NOT_FOUND = gettext("'%s' not found!");
char *MULTIBOOKMARKS_DEFAULT = gettext("Default Bookmark File");
char *MULTIBOOKMARKS_SMALL = gettext("Screen too small! (8x35 min)");
char *MULTIBOOKMARKS_SAVE = gettext("Select destination or ^G to Cancel: ");
/* #define MULTIBOOKMARKS_SELECT */char *MULTIBOOKMARKS_SELECT = \
 gettext("Select subbookmark, '=' for menu, or ^G to cancel: ");
/* #define MULTIBOOKMARKS_SELF */char *MULTIBOOKMARKS_SELF = \
 gettext("Reproduce L)ink in this bookmark file or C)ancel? (l,c): ");
char *MULTIBOOKMARKS_DISALLOWED = gettext("Multiple bookmark support is not available.");
char *MULTIBOOKMARKS_SHEAD_MASK = gettext(" Select Bookmark (screen %d of %d)");
char *MULTIBOOKMARKS_SHEAD = gettext("       Select Bookmark");
/* #define MULTIBOOKMARKS_EHEAD_MASK */char *MULTIBOOKMARKS_EHEAD_MASK = \
 gettext("Editing Bookmark DESCRIPTION and FILEPATH (%d of 2)");
/* #define MULTIBOOKMARKS_EHEAD */char *MULTIBOOKMARKS_EHEAD = \
 gettext("         Editing Bookmark DESCRIPTION and FILEPATH");
char *MULTIBOOKMARKS_LETTER = gettext("Letter: ");
#ifdef VMS
/* #define USE_PATH_OFF_HOME */char *USE_PATH_OFF_HOME = \
 gettext("Use a filepath off your login directory in SHELL syntax!");
#else
char *USE_PATH_OFF_HOME = gettext("Use a filepath off your home directory!");
#endif /* VMS */
/* #define MAXLINKS_REACHED */char *MAXLINKS_REACHED = \
 gettext("Maximum links per page exceeded!  Use half-page or two-line scrolling.");
char *VISITED_LINKS_EMPTY = gettext("No previously visited links available!");
char *MEMORY_EXHAUSTED_ABORT = gettext("Memory exhausted!  Program aborted!");
char *MEMORY_EXHAUSTED_ABORTING = gettext("Memory exhausted!  Aborting...");
char *NOT_ENOUGH_MEMORY = gettext("Not enough memory!");
char *DFM_NOT_AVAILABLE = gettext("Directory/File Manager not available");
char *LOCATION_NOT_ABSOLUTE = gettext("Location URL is not absolute.");
char *REFRESH_URL_NOT_ABSOLUTE = gettext("Refresh URL is not absolute.");
/* #define SENDING_MESSAGE_WITH_BODY_TO */char *SENDING_MESSAGE_WITH_BODY_TO = \
 gettext("You are sending a message with body to:\n  ");
char *SENDING_COMMENT_TO = gettext("You are sending a comment to:\n  ");
char *WITH_COPY_TO = gettext("\n With copy to:\n  ");
char *WITH_COPIES_TO = gettext("\n With copies to:\n  ");
/* #define CTRL_G_TO_CANCEL_SEND */char *CTRL_G_TO_CANCEL_SEND = \
 gettext("\n\nUse Ctrl-G to cancel if you do not want to send a message\n");
/* #define ENTER_NAME_OR_BLANK */char *ENTER_NAME_OR_BLANK = \
 gettext("\n Please enter your name, or leave it blank to remain anonymous\n");
/* #define ENTER_MAIL_ADDRESS_OR_OTHER */char *ENTER_MAIL_ADDRESS_OR_OTHER = \
 gettext("\n Please enter a mail address or some other\n");
/* #define MEANS_TO_CONTACT_FOR_RESPONSE */char *MEANS_TO_CONTACT_FOR_RESPONSE = \
 gettext(" means to contact you, if you desire a response.\n");
char *ENTER_SUBJECT_LINE = gettext("\n Please enter a subject line.\n");
/* #define ENTER_ADDRESS_FOR_CC */char *ENTER_ADDRESS_FOR_CC = \
 gettext("\n Enter a mail address for a CC of your message.\n");
char *BLANK_FOR_NO_COPY = gettext(" (Leave blank if you don't want a copy.)\n");
char *REVIEW_MESSAGE_BODY = gettext("\n Please review the message body:\n\n");
char *RETURN_TO_CONTINUE = gettext("\nPress RETURN to continue: ");
char *RETURN_TO_CLEANUP = gettext("\nPress RETURN to clean up: ");
char *CTRL_U_TO_ERASE = gettext(" Use Control-U to erase the default.\n");
char *ENTER_MESSAGE_BELOW = gettext("\n Please enter your message below.");
/* #define ENTER_PERIOD_WHEN_DONE_A */char *ENTER_PERIOD_WHEN_DONE_A = \
 gettext("\n When you are done, press enter and put a single period (.)");
/* #define ENTER_PERIOD_WHEN_DONE_B */char *ENTER_PERIOD_WHEN_DONE_B = \
 gettext("\n on a line and press enter again.");

/* Cookies messages */
/* #define ADVANCED_COOKIE_CONFIRMATION */char *ADVANCED_COOKIE_CONFIRMATION = \
 gettext("%s cookie: %.*s=%.*s  Allow? (Y/N/Always/neVer)");
/* #define INVALID_COOKIE_DOMAIN_CONFIRMATION */char *INVALID_COOKIE_DOMAIN_CONFIRMATION = \
 gettext("Accept invalid cookie domain=%s for '%s'?");
/* #define INVALID_COOKIE_PATH_CONFIRMATION */char *INVALID_COOKIE_PATH_CONFIRMATION = \
 gettext("Accept invalid cookie path=%s as a prefix of '%s'?");
char *ALLOWING_COOKIE = gettext("Allowing this cookie.");
char *REJECTING_COOKIE = gettext("Rejecting this cookie.");
char *COOKIE_JAR_IS_EMPTY = gettext("The Cookie Jar is empty.");
char *CACHE_JAR_IS_EMPTY = gettext("The Cache Jar is empty.");
/* #define ACTIVATE_TO_GOBBLE */char *ACTIVATE_TO_GOBBLE = \
 gettext("Activate links to gobble up cookies or entire domains,");
char *OR_CHANGE_ALLOW = gettext("or to change a domain's 'allow' setting.");
char *COOKIES_NEVER_ALLOWED = gettext("(Cookies never allowed.)");
char *COOKIES_ALWAYS_ALLOWED = gettext("(Cookies always allowed.)");
char *COOKIES_ALLOWED_VIA_PROMPT = gettext("(Cookies allowed via prompt.)");
char *COOKIES_READ_FROM_FILE = gettext("(Persistent Cookies.)");
char *NO_TITLE = gettext("(No title.)");
char *NO_NAME = gettext("(No name.)");
char *NO_VALUE = gettext("(No value.)");
char *NO_NOTHING = gettext("None");
char *END_OF_SESSION = gettext("(End of session.)");
char *DELETE_COOKIE_CONFIRMATION = gettext("Delete this cookie?");
char *COOKIE_EATEN = gettext("The cookie has been eaten!");
char *DELETE_EMPTY_DOMAIN_CONFIRMATION = gettext("Delete this empty domain?");
char *DOMAIN_EATEN = gettext("The domain has been eaten!");
/* #define DELETE_COOKIES_SET_ALLOW_OR_CANCEL */char *DELETE_COOKIES_SET_ALLOW_OR_CANCEL = \
 gettext("D)elete domain's cookies, set allow A)lways/P)rompt/neV)er, or C)ancel? ");
/* #define DELETE_DOMAIN_SET_ALLOW_OR_CANCEL */char *DELETE_DOMAIN_SET_ALLOW_OR_CANCEL = \
 gettext("D)elete domain, set allow A)lways/P)rompt/neV)er, or C)ancel? ");
char *DOMAIN_COOKIES_EATEN = gettext("All cookies in the domain have been eaten!");
char *ALWAYS_ALLOWING_COOKIES = gettext("'A'lways allowing from domain '%s'.");
char *NEVER_ALLOWING_COOKIES = gettext("ne'V'er allowing from domain '%s'.");
char *PROMPTING_TO_ALLOW_COOKIES = gettext("'P'rompting to allow from domain '%s'.");
char *DELETE_ALL_COOKIES_IN_DOMAIN = gettext("Delete all cookies in this domain?");
char *ALL_COOKIES_EATEN = gettext("All of the cookies in the jar have been eaten!");

char *PORT_NINETEEN_INVALID = gettext("Port 19 not permitted in URLs.");
char *PORT_TWENTYFIVE_INVALID = gettext("Port 25 not permitted in URLs.");
char *PORT_INVALID = gettext("Port %lu not permitted in URLs.");
char *URL_PORT_BAD = gettext("URL has a bad port field.");
char *HTML_STACK_OVERRUN = gettext("Maximum nesting of HTML elements exceeded.");
char *BAD_PARTIAL_REFERENCE = gettext("Bad partial reference!  Stripping lead dots.");
char *TRACELOG_OPEN_FAILED = gettext("Trace Log open failed.  Trace off!");
char *LYNX_TRACELOG_TITLE = gettext("Lynx Trace Log");
char *NO_TRACELOG_STARTED = gettext("No trace log has been started for this session.");
/* #define MAX_TEMPCOUNT_REACHED */char *MAX_TEMPCOUNT_REACHED = \
 gettext("The maximum temporary file count has been reached!");
/* #define FORM_VALUE_TOO_LONG */char *FORM_VALUE_TOO_LONG = \
 gettext("Form field value exceeds buffer length!  Trim the tail.");
/* #define FORM_TAIL_COMBINED_WITH_HEAD */char *FORM_TAIL_COMBINED_WITH_HEAD = \
 gettext("Modified tail combined with head of form field value.");

/* HTFile.c */
char *ENTRY_IS_DIRECTORY = gettext("Directory");
char *DISALLOWED_DIR_SCAN = gettext("Directory browsing is not allowed.");
char *DISALLOWED_SELECTIVE_ACCESS = gettext("Selective access is not enabled for this directory");
char *FAILED_DIR_SCAN = gettext("Multiformat: directory scan failed.");
char *FAILED_DIR_UNREADABLE = gettext("This directory is not readable.");
char *FAILED_FILE_UNREADABLE = gettext("Can't access requested file.");
char *FAILED_NO_REPRESENTATION = gettext("Could not find suitable representation for transmission.");
char *FAILED_OPEN_COMPRESSED_FILE = gettext("Could not open file for decompression!");
char *LABEL_FILES = gettext("Files:");
char *LABEL_SUBDIRECTORIES = gettext("Subdirectories:");
char *SEGMENT_DIRECTORY = gettext(" directory");
char *SEGMENT_UP_TO = gettext("Up to ");
char *SEGMENT_CURRENT_DIR = gettext("Current directory is ");

/* HTFTP.c */
char *ENTRY_IS_SYMBOLIC_LINK = gettext("Symbolic Link");

/* HTGopher.c */
char *FAILED_NO_RESPONSE = gettext("No response from server!");
char *GOPHER_CSO_INDEX = gettext("CSO index");
char *GOPHER_CSO_INDEX_SUBTITLE = gettext("\nThis is a searchable index of a CSO database.\n");
char *GOPHER_CSO_SEARCH_RESULTS = gettext("CSO Search Results");
char *GOPHER_CSO_SEEK_FAILED = gettext("Seek fail on %s\n");
char *GOPHER_CSO_SOLICIT_KEYWORDS = gettext("\nPress the 's' key and enter search keywords.\n");
char *GOPHER_INDEX_SUBTITLE = gettext("\nThis is a searchable Gopher index.\n");
char *GOPHER_INDEX_TITLE = gettext("Gopher index");
char *GOPHER_MENU_TITLE = gettext("Gopher Menu");
char *GOPHER_SEARCH_RESULTS = gettext(" Search Results");
char *GOPHER_SENDING_CSO_REQUEST = gettext("Sending CSO/PH request.");
char *GOPHER_SENDING_REQUEST = gettext("Sending Gopher request.");
char *GOPHER_SENT_CSO_REQUEST = gettext("CSO/PH request sent; waiting for response.");
char *GOPHER_SENT_REQUEST = gettext("Gopher request sent; waiting for response.");
char *GOPHER_SOLICIT_KEYWORDS = gettext("\nPlease enter search keywords.\n");
char *SEGMENT_KEYWORDS_WILL = gettext("\nThe keywords that you enter will allow you to search on a");
char *SEGMENT_PERSONS_DB_NAME = gettext(" person's name in the database.\n");

/* HTNews.c */
char *FAILED_CONNECTION_CLOSED = gettext("Connection closed ???");
char *FAILED_CANNOT_OPEN_POST = gettext("Cannot open temporary file for news POST.");
char *FAILED_CANNOT_POST_SSL = gettext("This client does not contain support for posting to news with SSL.");

/* HTStyle.c */
char *STYLE_DUMP_FONT = gettext("Style %d `%s' SGML:%s.  Font %s %.1f point.\n");
char *STYLE_DUMP_INDENT = gettext("\tIndents: first=%.0f others=%.0f, Height=%.1f Desc=%.1f\n"
char *STYLE_DUMP_ALIGN = gettext("\tAlign=%d, %d tabs. (%.0f before, %.0f after)\n");
char *STYLE_DUMP_TAB = gettext("\t\tTab kind=%d at %.0f\n");

/* HTTP.c */
char *FAILED_NEED_PASSWD = gettext("Can't proceed without a username and password.");
char *FAILED_RETRY_WITH_AUTH = gettext("Can't retry with authorization!  Contact the server's WebMaster.");
char *FAILED_RETRY_WITH_PROXY = gettext("Can't retry with proxy authorization!  Contact the server's WebMaster.");
char *HTTP_RETRY_WITH_PROXY = gettext("Retrying with proxy authorization information.");
char *SSL_FORCED_PROMPT = gettext("SSL error:%s-Continue?");

/* HTWAIS.c */
char *HTWAIS_MESSAGE_TOO_BIG = gettext("HTWAIS: Return message too large.");
char *HTWAIS_SOLICIT_QUERY = gettext("Enter WAIS query: ");

/* Miscellaneous status */
char *RETRYING_AS_HTTP0 = gettext("Retrying as HTTP0 request.");
char *TRANSFERRED_X_BYTES = gettext("Transferred %d bytes");
char *TRANSFER_COMPLETE = gettext("Data transfer complete");
char *FAILED_READING_KEYMAP = gettext("Error processing line %d of %s\n");

/* Lynx internal page titles */
char *ADDRLIST_PAGE_TITLE = gettext("Address List Page");
char *BOOKMARK_TITLE = gettext("Bookmark file");
char *CONFIG_DEF_TITLE = gettext("Configuration Definitions");
char *COOKIE_JAR_TITLE = gettext("Cookie Jar");
char *CURRENT_EDITMAP_TITLE = gettext("Current Edit-Key Map");
char *CURRENT_KEYMAP_TITLE = gettext("Current Key Map");
char *DIRED_MENU_TITLE = gettext("File Management Options");
char *DOWNLOAD_OPTIONS_TITLE = gettext("Download Options");
char *HISTORY_PAGE_TITLE = gettext("History Page");
char *CACHE_JAR_TITLE = gettext("Cache Jar");
char *LIST_PAGE_TITLE = gettext("List Page");
char *LYNXCFG_TITLE = gettext("Lynx.cfg Information");
char *MOSAIC_BOOKMARK_TITLE = gettext("Converted Mosaic Hotlist");
char *OPTIONS_TITLE = gettext("Options Menu");
char *PERMIT_OPTIONS_TITLE = gettext("File Permission Options");
char *PRINT_OPTIONS_TITLE = gettext("Printing Options");
char *SHOWINFO_TITLE = gettext("Information about the current document");
char *STATUSLINES_TITLE = gettext("Your recent statusline messages");
char *UPLOAD_OPTIONS_TITLE = gettext("Upload Options");
char *VISITED_LINKS_TITLE = gettext("Visited Links Page");

/* CONFIG_DEF_TITLE subtitles */
char *SEE_ALSO = gettext("See also");
char *YOUR_SEGMENT = gettext("your");
char *RUNTIME_OPT_SEGMENT = gettext("for runtime options");
char *COMPILE_OPT_SEGMENT = gettext("compile time options");
char *COLOR_STYLE_SEGMENT = gettext("color-style configuration");
char *REL_VERSION = gettext("latest release");
char *PRE_VERSION = gettext("pre-release version");
char *DEV_VERSION = gettext("development version");
/* #define AUTOCONF_CONFIG_CACHE */char *AUTOCONF_CONFIG_CACHE = \
 gettext("The following data were derived during the automatic configuration/build\n\
process of this copy of Lynx.  When reporting a bug, please include a copy\n\
of this page.");
/* #define AUTOCONF_LYNXCFG_H */char *AUTOCONF_LYNXCFG_H = \
 gettext("The following data were used as automatically-configured compile-time\n\
definitions when this copy of Lynx was built.");

#ifdef DIRED_SUPPORT
/* #define DIRED_NOVICELINE */char *DIRED_NOVICELINE = \
 gettext("  C)reate  D)ownload  E)dit  F)ull menu  M)odify  R)emove  T)ag  U)pload     \n");
char *CURRENT_LINK_STATUS_FAILED = gettext("Failed to obtain status of current link!");

/* #define INVALID_PERMIT_URL */char *INVALID_PERMIT_URL = \
 gettext("Special URL only valid from current File Permission menu!");
#endif /* DIRED_SUPPORT */

#ifdef USE_EXTERNALS
char *EXTERNALS_DISABLED = gettext("External support is currently disabled.");
#endif /* USE_EXTERNALS */

/* new with 2.8.4dev.21 */
char *CHDIR_DISABLED = gettext("Changing working-directory is currently disabled.");
char *LINEWRAP_OFF = gettext("Linewrap OFF!");
char *LINEWRAP_ON = gettext("Linewrap ON!");
char *NESTED_TABLES_OFF = gettext("Parsing nested-tables toggled OFF!  Reloading...");
char *NESTED_TABLES_ON = gettext("Parsing nested-tables toggled ON!  Reloading...");
char *SHIFT_VS_LINEWRAP = gettext("Shifting is disabled while line-wrap is in effect");
char *TRACE_DISABLED = gettext("Trace not supported");

#endif /* LYMESSAGES_EN_H */
