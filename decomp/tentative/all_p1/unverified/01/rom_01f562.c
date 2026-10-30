/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01f562.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1f562(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_146e8();
  candidate_9f88();
  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8844;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8844;
  candidate_40bc();
  candidate_1acf2();
  candidate_4256();
  *unaff_A6 = 0x1f5c8;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  uVar1 = *(ushort *)(unaff_A6[3] + 0x6e);
  if ((uVar1 == 1) || (uVar2 = uVar1 < 2, uVar1 == 2)) {
    *(ushort *)((int)unaff_A6 + 0x6e) = uVar1;
    *unaff_A6 = candidate_1f636;
    uVar2 = false;
  }
  candidate_4c24();
  if ((bool)uVar2) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar2 = 0;
    }
    else {
      *unaff_A6 = candidate_1f6c0;
      uVar2 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_1f7ea;
  }
  return;
}
