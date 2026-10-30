/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d8a4c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d8a4c(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_4256();
  uVar3 = candidate_29c6();
  *(ushort *)(unaff_A6 + 10) = ((uVar3 & 7) - 4) + *(short *)(unaff_A6 + 10);
  *(undefined2 *)(unaff_A6 + 0xb) = 0xa0;
  uVar4 = 0;
  candidate_146e8();
  candidate_1abc8();
  candidate_19504();
  if (!(bool)uVar4) {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
    candidate_9ea8();
    *unaff_A6 = candidate_d8ab6;
    return;
  }
  candidate_4192();
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
