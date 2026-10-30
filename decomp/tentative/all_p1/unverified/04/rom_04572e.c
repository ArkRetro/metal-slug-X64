/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04572e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4572e(void)

{
  short *psVar1;
  char cVar2;
  undefined4 *unaff_A6;

  if (*(short *)(unaff_A6 + 0xf) < 0x128) {
    *(undefined1 *)((int)unaff_A6 + 0x8a) = 1;
    *unaff_A6 = 0x45744;
  }
  candidate_1abc8();
  candidate_427a();
  if ((*(char *)((int)unaff_A6 + 0x8a) != '\0') &&
     (cVar2 = *(char *)((int)unaff_A6 + 0x8a) + -1, *(char *)((int)unaff_A6 + 0x8a) = cVar2,
     cVar2 == '\0')) {
    *(undefined1 *)((int)unaff_A6 + 0x8a) = 0x2c;
    cVar2 = *(char *)((int)unaff_A6 + 0x8b) + -1;
    *(char *)((int)unaff_A6 + 0x8b) = cVar2;
    if (-1 < cVar2) {
      candidate_120f8();
    }
  }
  psVar1 = *(short **)((int)unaff_A6 + 0x86);
  if (*(short *)(unaff_A6[3] + 0x8a) == *psVar1) {
    *(char *)(unaff_A6 + 0x23) = (char)*psVar1;
    *unaff_A6 = *(undefined4 *)(psVar1 + 1);
    *(short **)((int)unaff_A6 + 0x86) = psVar1 + 3;
  }
  return;
}
