/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0700da.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_700da(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  candidate_4256();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
  *(undefined4 *)((int)unaff_A6 + 0x6e) = 0;
  candidate_353a4();
  candidate_1b1dc();
  *(undefined2 *)(unaff_A6 + 0x20) = 0x8080;
  *(undefined1 *)((int)unaff_A6 + 0x86) = 0xff;
  *unaff_A6 = 0x7011a;
  uVar2 = 0;
  candidate_70e34();
  candidate_70db6();
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_70590;
    uVar2 = 0;
  }
  uVar1 = candidate_aa610();
  if ((bool)uVar2) {
    *(undefined2 *)((int)unaff_A6 + 0x7a) = uVar1;
    bVar3 = false;
    uVar1 = candidate_aa760();
    if (bVar3) {
      *(undefined2 *)(unaff_A6 + 0x1f) = uVar1;
      candidate_aa83a();
      *unaff_A6 = candidate_70182;
    }
  }
  *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + 1;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  return;
}
