/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04ddd4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4ddd4(void)

{
  int iVar1;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A6 + 0xc);
  *(undefined4 *)(unaff_A6 + 0x6e) = *(undefined4 *)(iVar1 + 0x6e);
  if (*(char *)(iVar1 + 0x9e) == -1) {
    if (*(char *)(unaff_A6 + 0xa1) == '\0') {
      if (*(char *)(iVar1 + 0x9d) == '\x03') {
        if (*(char *)(unaff_A6 + 0x9d) != '\x03') {
          *(undefined1 *)(iVar1 + 0x9e) = 0x7f;
          *(undefined1 *)(unaff_A6 + 0x9e) = 0;
          *(undefined1 *)(unaff_A6 + 0x9d) = *(undefined1 *)(iVar1 + 0x9d);
          return;
        }
        *(undefined1 *)(iVar1 + 0x9e) = 0;
      }
      if (*(char *)(iVar1 + 0x9d) == '\x04') {
        if (*(char *)(unaff_A6 + 0x9d) != '\x04') {
          *(undefined1 *)(iVar1 + 0x9e) = 0x7f;
          *(undefined1 *)(unaff_A6 + 0x9e) = 0;
          *(undefined1 *)(unaff_A6 + 0x9d) = *(undefined1 *)(iVar1 + 0x9d);
          return;
        }
        *(undefined1 *)(iVar1 + 0x9e) = 0;
      }
      return;
    }
    if (*(char *)(unaff_A6 + 0xa1) == '\x01') {
      if ((*(char *)(iVar1 + 0x9d) == '\x03') && (*(char *)(unaff_A6 + 0x9d) != '\x03')) {
        *(undefined1 *)(unaff_A6 + 0x9d) = *(undefined1 *)(iVar1 + 0x9d);
        return;
      }
      if ((*(char *)(iVar1 + 0x9d) == '\x04') && (*(char *)(unaff_A6 + 0x9d) != '\x04')) {
        *(undefined1 *)(unaff_A6 + 0x9d) = *(undefined1 *)(iVar1 + 0x9d);
        return;
      }
      return;
    }
    if (*(char *)(unaff_A6 + 0xa1) == '\x02') {
      if ((*(char *)(iVar1 + 0x9d) == '\x03') && (*(char *)(unaff_A6 + 0x9d) != '\x03')) {
        *(undefined1 *)(unaff_A6 + 0x9d) = *(undefined1 *)(iVar1 + 0x9d);
        return;
      }
      if ((*(char *)(iVar1 + 0x9d) == '\x04') && (*(char *)(unaff_A6 + 0x9d) != '\x04')) {
        *(undefined1 *)(unaff_A6 + 0x9d) = *(undefined1 *)(iVar1 + 0x9d);
        return;
      }
      return;
    }
  }
  return;
}
