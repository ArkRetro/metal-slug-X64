/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04e2ea.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4e2ea(void)

{
  code *pcVar1;
  int iVar2;
  byte *pbVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0e4;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0e4;
  candidate_1ad6e();
  if ((*(byte *)(*(int *)(unaff_A6[3] + 0xc) + 0xa0) & 8) == 0) {
    candidate_4256();
  }
  else if ((*(byte *)(*(int *)(unaff_A6[3] + 0xc) + 0xa0) & 0x10) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *(undefined1 *)((int)unaff_A6 + 0x9e) = 0;
  *unaff_A6 = 0x4e376;
  if ((*(byte *)(*(int *)(unaff_A6[3] + 0xc) + 0x60) & 8) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x9e) = 0xff;
  }
  uVar4 = 0;
  candidate_1abc8();
  candidate_427a();
  if (((bool)uVar4) && ((*(byte *)(*(int *)(unaff_A6[3] + 0xc) + 0x60) & 8) != 0)) {
    pbVar3 = (byte *)(*(int *)(unaff_A6[3] + 0xc) + 0x60);
    *pbVar3 = *pbVar3 & 0xf7;
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
