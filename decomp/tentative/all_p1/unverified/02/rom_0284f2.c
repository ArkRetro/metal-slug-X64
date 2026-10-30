/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0284f2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_284f2(void)

{
  undefined4 *unaff_A6;
  bool bVar1;
  undefined1 uVar2;

  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  candidate_1b1dc();
  bVar1 = false;
  candidate_40bc();
  if (bVar1) {
    candidate_1b220();
  }
  *(undefined1 *)(unaff_A6 + 0x18) = 1;
  candidate_4256();
  *unaff_A6 = 0x2853e;
  uVar2 = 0;
  candidate_1436a();
  candidate_427a();
  if (((bool)uVar2) && (uVar2 = 0, *(char *)(unaff_A6 + 0x18) == '\0')) {
    *unaff_A6 = candidate_28572;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_28660;
  }
  return;
}
