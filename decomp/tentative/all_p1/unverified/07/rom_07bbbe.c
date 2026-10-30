/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07bbbe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7bbbe(void)

{
  ushort uVar1;
  ushort uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_1abc8();
  candidate_427a();
  uVar1 = *(ushort *)(unaff_A6 + 0xf);
  uVar2 = *(ushort *)((int)unaff_A6 + 0x82);
  if ((short)uVar2 >= (short)uVar1) {
    *unaff_A6 = candidate_7bbe0;
  }
  uVar3 = (short)uVar2 < (short)uVar1 && uVar1 < uVar2;
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = 0;
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_7be22;
      uVar3 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_7be86;
  }
  return;
}
