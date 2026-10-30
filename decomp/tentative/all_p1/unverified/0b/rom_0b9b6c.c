/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b9b6c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b9b6c(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *unaff_A6 = 0xb9b7a;
  uVar3 = unaff_A6[0xd];
  uVar4 = 0x4fff < uVar3;
  unaff_A6[0xd] = uVar3 - 0x5000;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_b9ba2;
    uVar4 = 0;
  }
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
