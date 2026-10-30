/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x089db4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x00089e2c: Changing call to branch */

void candidate_89db4(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  byte bVar5;
  undefined2 uVar4;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;
  undefined4 uStack_4;

  uStack_4 = 0x89dd0;
  candidate_4256();
  *(undefined1 *)(unaff_A6 + 0x22) = 1;
  *(undefined2 *)(unaff_A6 + 0x1e) = 0;
  *unaff_A6 = 0x89de0;
  uStack_4 = 0x89de6;
  candidate_1436a();
  uStack_4 = 0x89dec;
  candidate_427a();
  sVar3 = *(short *)(unaff_A6 + 0x1e);
  *(ushort *)(unaff_A6 + 0x1e) = sVar3 + 1U;
  bVar5 = (char)((ushort)(sVar3 + 1U) / 6) + 1;
  if (bVar5 < 6) {
    *(byte *)(unaff_A6 + 0x23) = bVar5;
    register0x0000003c = (BADSPACEBASE *)&uStack_4;
    uStack_4 = 0x89e30;
  }
  else {
    uStack_4 = 0x89e12;
    uVar4 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x1e) = uVar4;
    uStack_4 = 0x89e24;
    candidate_8a5dc();
    *(undefined1 *)((int)unaff_A6 + 0x89) = 0;
    bVar6 = false;
    uStack_4 = 0x8a1ca;
    candidate_1945c();
    if (bVar6) {
      uStack_4 = 0x41ae;
      candidate_4192();
      uStack_4 = 0x418a;
      candidate_4126();
      iVar2 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar2 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
    uStack_4 = 0x8a1d8;
    candidate_8a654();
    *(char *)((int)unaff_A6 + 0x8e) = -bVar6;
    if (bVar6) {
      *(undefined1 *)((int)unaff_A6 + 0x8a) = 1;
      *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
      *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
    }
    else {
      *(undefined2 *)((int)unaff_A6 + 0x66) = *(undefined2 *)(unaff_A6 + 0x20);
      *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6 + 0x20);
    }
    uStack_4 = 0x8a208;
    candidate_1ad8c();
    if (*(char *)((int)unaff_A6 + 0x8a) == '\0') {
      uStack_4 = 0x8a216;
      candidate_4c24();
    }
    else {
      *(char *)((int)unaff_A6 + 0x8a) = *(char *)((int)unaff_A6 + 0x8a) + -1;
    }
    if (((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) &&
       (*unaff_A6 = candidate_89e34, *(char *)((int)unaff_A6 + 0x5a) == '\a')) {
      *unaff_A6 = candidate_89ed6;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    if (*(char *)((int)unaff_A6 + 0x85) != '\0') {
      *(char *)((int)unaff_A6 + 0x85) = *(char *)((int)unaff_A6 + 0x85) + -1;
    }
    if (*(short *)((int)unaff_A6 + 0x82) < *(short *)(unaff_A6 + 10)) {
      *unaff_A6 = candidate_8a00a;
    }
  }
  if (*(char *)(unaff_A6 + 0x23) != *(char *)((int)unaff_A6 + 0x8d)) {
    *(char *)((int)unaff_A6 + 0x8d) = *(char *)(unaff_A6 + 0x23);
    *(undefined4 *)((int)register0x0000003c + -4) = 0x8a644;
    candidate_9d1e();
  }
  return;
}
