/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09a0f6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9a0f6(void)

{
  undefined1 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  unaff_A6[0x14] = 0x9ad4c;
  candidate_4256();
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *unaff_A6 = 0x9a11c;
  uVar2 = 0;
  candidate_143dc();
  candidate_1ad8c();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = 0x9a042;
    uVar2 = 0;
  }
  candidate_427a();
  uVar1 = candidate_9ab9a();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x83) = uVar1;
    *unaff_A6 = candidate_9a17e;
    uVar2 = 0;
  }
  candidate_c8bec();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_9a5ee;
  }
  return;
}
