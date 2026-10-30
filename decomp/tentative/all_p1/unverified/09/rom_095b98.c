/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x095b98.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_95b98(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int unaff_A6;

  cVar1 = *(char *)(unaff_A6 + 0x92) + -1;
  *(char *)(unaff_A6 + 0x92) = cVar1;
  if (cVar1 == '\0') {
    *(byte *)(unaff_A6 + 0x91) = *(char *)(unaff_A6 + 0x93) + *(char *)(unaff_A6 + 0x91) & 0xf;
    candidate_4256();
    if ((*(char *)(unaff_A6 + 0x91) == '\x04') && (-1 < *(char *)(unaff_A6 + 0x93))) {
      *(undefined1 *)(unaff_A6 + 0x96) = 0xff;
      candidate_120f8();
    }
    if (*(char *)(unaff_A6 + 0x97) == '\0') {
      *(undefined1 *)(unaff_A6 + 0x92) = 2;
    }
    else {
      if (*(ushort *)(unaff_A6 + 0x86) < 2) {
        iVar2 = 0x95b24;
        iVar3 = 0x95b14;
      }
      else {
        iVar2 = 0x95b34;
        iVar3 = 0x95b24;
      }
      if (0x27 < *(short *)(unaff_A6 + 0x88)) {
        iVar2 = iVar3;
      }
      *(char *)(unaff_A6 + 0x92) =
           (char)(CONCAT11(*(char *)(unaff_A6 + 0x91) >> 7,
                           *(undefined1 *)(iVar2 + (short)*(char *)(unaff_A6 + 0x91))) >> 1);
    }
  }
  return;
}
