/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09a3d4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9a3d4(void)

{
  ushort uVar1;
  undefined1 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_4256();
  unaff_A6[0xd] = 0x15500;
  *unaff_A6 = 0x9a3f2;
  uVar1 = candidate_402e6();
  if ((short)uVar1 < 0) {
    *unaff_A6 = 0x9a000;
    uVar3 = false;
  }
  else {
    uVar3 = uVar1 < 0x24;
    if ((bool)uVar3) {
      *unaff_A6 = candidate_9a438;
      uVar3 = false;
    }
  }
  candidate_427a();
  if ((bool)uVar3) {
    uVar3 = 0;
    candidate_4256();
  }
  uVar2 = candidate_9ab9a();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x83) = uVar2;
    *unaff_A6 = candidate_9a17e;
    uVar3 = 0;
  }
  candidate_c8bec();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_9a5ee;
  }
  return;
}
