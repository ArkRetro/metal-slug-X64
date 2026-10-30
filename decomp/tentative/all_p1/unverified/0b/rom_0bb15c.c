/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bb15c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_bb15c(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  uVar3 = unaff_A6[0xd];
  uVar4 = 0x9fff < uVar3;
  unaff_A6[0xd] = uVar3 - 0xa000;
  candidate_1ac38();
  candidate_427a();
  candidate_1945c();
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
