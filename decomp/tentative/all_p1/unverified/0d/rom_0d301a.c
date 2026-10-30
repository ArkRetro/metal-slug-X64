/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d301a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d301a(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_9ea8();
  uVar3 = candidate_29c6();
  *(ushort *)(unaff_A6 + 10) = ((uVar3 & 0xf) - 8) + *(short *)(unaff_A6 + 10);
  candidate_4256();
  *unaff_A6 = 0xd305a;
  uVar4 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar4) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
