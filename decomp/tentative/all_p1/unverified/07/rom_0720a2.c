/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0720a2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_720a2(void)

{
  code *pcVar1;
  int iVar2;
  undefined2 uVar3;
  undefined2 extraout_D1w;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_4256();
  candidate_9ea8();
  candidate_1ad6e();
  *unaff_A6 = 0x720d0;
  bVar4 = false;
  candidate_409a();
  if (bVar4) {
    candidate_1aca2();
    *(undefined1 *)((int)unaff_A6 + 0x87) = *(undefined1 *)(extraout_A0 + 0x87);
    *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(extraout_A0 + 0x1e);
    *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + -1;
    uVar3 = candidate_72770();
    *(undefined2 *)(unaff_A6 + 10) = uVar3;
    *(undefined2 *)(unaff_A6 + 0xb) = extraout_D1w;
    candidate_1abc8();
    candidate_427a();
    return;
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
