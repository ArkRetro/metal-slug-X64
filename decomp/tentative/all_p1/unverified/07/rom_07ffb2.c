/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07ffb2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7ffb2(void)

{
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  candidate_4256();
  candidate_9ea8();
  candidate_1dea2();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfd;
  unaff_A6[0x14] = 0;
  *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
  *unaff_A6 = 0x7ffec;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_80038;
  }
  if (*(char *)((int)unaff_A6 + 0x93) != '\0') {
    candidate_49e8();
    if (extraout_A0 != 0) {
      *(undefined1 *)((int)unaff_A6 + 0x93) = 0;
    }
  }
  bVar2 = false;
  candidate_19504();
  if (bVar2) {
    *unaff_A6 = candidate_80038;
  }
  return;
}
