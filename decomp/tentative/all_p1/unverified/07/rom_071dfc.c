/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x071dfc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_71dfc(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  char cVar4;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar5;

  candidate_1ad6e();
  candidate_1aca2();
  *(undefined2 *)((int)unaff_A6 + 0x72) = *(undefined2 *)(extraout_A0 + 0x72);
  *(undefined2 *)(unaff_A6 + 0x1d) = *(undefined2 *)(extraout_A0 + 0x74);
  cVar4 = candidate_c05b8();
  if (cVar4 == '\x01') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  candidate_9ea8();
  *unaff_A6 = 0x71e86;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  sVar3 = *(short *)((int)unaff_A6 + 0x1e);
  uVar5 = sVar3 == 0;
  *(short *)((int)unaff_A6 + 0x1e) = sVar3 + -1;
  candidate_1abc8();
  candidate_427a();
  if (!(bool)uVar5) {
    return;
  }
  *(undefined1 *)(unaff_A6[3] + 0x60) = 0xff;
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
