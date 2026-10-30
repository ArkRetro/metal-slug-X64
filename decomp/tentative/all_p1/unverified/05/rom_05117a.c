/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05117a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5117a(void)

{
  char *pcVar1;
  char cVar2;
  int unaff_A6;

  if (4 < *(ushort *)(unaff_A6 + 0x8e)) {
    return;
  }
  if (*(char *)(unaff_A6 + 0x9f) != '\0') {
    return;
  }
  if ((*(int *)(unaff_A6 + 0x7a) != 0) &&
     (cVar2 = *(char *)(unaff_A6 + 0x98) + -1, *(char *)(unaff_A6 + 0x98) = cVar2, cVar2 == '\0')) {
    pcVar1 = *(char **)(unaff_A6 + 0x7a);
    if (*pcVar1 < '\0') {
      *(undefined4 *)(unaff_A6 + 0x7a) = 0;
      *(undefined1 *)(unaff_A6 + 0x14) = *(undefined1 *)(unaff_A6 + 0x19);
    }
    else {
      *(undefined1 *)(unaff_A6 + 0x14) =
           *(undefined1 *)(unaff_A6 + 0x19 + (int)(short)((byte)(*pcVar1 - 1) & 3));
      *(byte *)(unaff_A6 + 0x98) = (byte)pcVar1[1] >> 1;
      *(char **)(unaff_A6 + 0x7a) = pcVar1 + 2;
    }
  }
  return;
}
