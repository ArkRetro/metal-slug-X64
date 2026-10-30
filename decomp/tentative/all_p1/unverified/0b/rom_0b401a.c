/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b401a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_b401a(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_1abc8();
  candidate_427a();
  if ((bool)in_CF) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  uVar3 = candidate_29c6();
  uVar3 = *(ushort *)(unaff_A5 + -0x7ff2) & uVar3 & 0x1f;
  if (uVar3 == 0) {
    uVar3 = candidate_c8022();
  }
  return uVar3;
}
