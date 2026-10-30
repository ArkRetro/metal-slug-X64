/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x079482.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_79482(void)

{
  int iVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_4256();
  *unaff_A6 = 0x79498;
  candidate_1abc8();
  iVar1 = unaff_A6[3];
  *(undefined1 *)((int)unaff_A6 + 0x8b) = *(undefined1 *)(iVar1 + 0x8b);
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(iVar1 + 0x1e);
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + 8;
  if (*(char *)(iVar1 + 0x8a) == '\x01') {
    *unaff_A6 = 0x793fc;
  }
  if (*(char *)(iVar1 + 0x8a) == '\0') {
    *unaff_A6 = candidate_794f4;
  }
  uVar2 = 0;
  candidate_44f6();
  candidate_427a();
  candidate_79e06();
  candidate_7a032();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
