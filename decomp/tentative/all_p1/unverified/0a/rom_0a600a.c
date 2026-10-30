/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a600a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a600a(void)

{
  code *pcVar1;
  int iVar2;
  byte *pbVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  pbVar3 = (byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0xa2);
  *pbVar3 = *pbVar3 | 0x10;
  *unaff_A6 = 0xa601a;
  uVar4 = 0;
  candidate_a6a0c();
  candidate_a6a26();
  if (!(bool)uVar4) {
    pbVar3 = (byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0xa2);
    *pbVar3 = *pbVar3 | 0x20;
    *unaff_A6 = candidate_a603a;
  }
  candidate_1abc8();
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0x5f) & 0x80) != 0) {
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
