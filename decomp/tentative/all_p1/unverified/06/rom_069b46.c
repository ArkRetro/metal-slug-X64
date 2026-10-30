/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x069b46.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_69b46(void)

{
  short sVar1;
  byte bVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_4256();
  *unaff_A6 = 0x69b5c;
  uVar3 = 0;
  candidate_1436a();
  candidate_69d40();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_699bc;
  }
  bVar2 = *(char *)((int)unaff_A6 + 0x7e) + 1;
  if (0x23 < bVar2) {
    bVar2 = 0;
  }
  *(byte *)((int)unaff_A6 + 0x7e) = bVar2;
  uVar3 = false;
  if (*(short *)(unaff_A6 + 0x1f) != 0) {
    sVar1 = *(short *)(unaff_A6 + 0x1f);
    uVar3 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x1f) = sVar1 + -1;
  }
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_69b74;
    uVar3 = 0;
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = 0x69c9a;
  }
  *(byte *)((int)unaff_A6 + 0x7f) = *(byte *)((int)unaff_A6 + 0x7f) & 0xfd;
  return;
}
