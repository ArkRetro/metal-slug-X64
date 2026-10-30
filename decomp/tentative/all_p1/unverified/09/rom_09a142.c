/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09a142.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9a142(void)

{
  undefined1 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  unaff_A6[0xc] = 0;
  candidate_4256();
  *unaff_A6 = 0x9a160;
  uVar2 = 0;
  candidate_9ac86();
  candidate_427a();
  if ((bool)uVar2) {
    *(undefined2 *)((int)unaff_A6 + 0x7a) = 0x1e;
    *unaff_A6 = 0x9a042;
    uVar2 = 0;
  }
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
