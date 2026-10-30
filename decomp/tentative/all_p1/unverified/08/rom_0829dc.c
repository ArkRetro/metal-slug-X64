/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0829dc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_829dc(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1110;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1110;
  candidate_9ea8();
  if ((*(byte *)(unaff_A6[3] + 0x15) & 1) != 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  if ((*(byte *)(unaff_A6[3] + 0x15) & 2) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  candidate_1ad6e();
  *unaff_A6 = 0x82a48;
  uVar3 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar3) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  iVar2 = *(int *)((int)unaff_A6 + 0x6e);
  if (iVar2 != 0) {
    iVar2 = candidate_49e8();
  }
  return iVar2;
}
