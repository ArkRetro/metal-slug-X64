/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x045656.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_45656(void)

{
  short *psVar1;
  undefined4 *unaff_A6;

  if (*(char *)((int)unaff_A6 + 0x8d) != '\0') {
    if ((char)*(undefined2 *)(unaff_A6[3] + 0x8a) == *(char *)(unaff_A6 + 0x23)) {
      *(undefined1 *)(unaff_A6[3] + 0x91) = 1;
    }
    *unaff_A6 = 0x4567a;
  }
  candidate_1abc8();
  candidate_427a();
  psVar1 = *(short **)((int)unaff_A6 + 0x86);
  if (*(short *)(unaff_A6[3] + 0x8a) == *psVar1) {
    *(char *)(unaff_A6 + 0x23) = (char)*psVar1;
    *unaff_A6 = *(undefined4 *)(psVar1 + 1);
    *(short **)((int)unaff_A6 + 0x86) = psVar1 + 3;
  }
  return;
}
