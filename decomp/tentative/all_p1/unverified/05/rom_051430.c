/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x051430.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_51430(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  uint uVar4;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar5;
  undefined1 uVar6;

  bVar5 = false;
  if (*(char *)((int)unaff_A6 + 0xa2) != '\0') {
    cVar3 = *(char *)((int)unaff_A6 + 0xa2);
    bVar5 = cVar3 == '\0';
    *(char *)((int)unaff_A6 + 0xa2) = cVar3 + -1;
  }
  candidate_4c24();
  if (bVar5) {
    candidate_5100c();
    *(undefined1 *)(unaff_A6 + 0x27) = *(undefined1 *)((int)unaff_A6 + 0x5a);
    *(undefined1 *)((int)unaff_A6 + 0x9d) = *(undefined1 *)((int)unaff_A6 + 0x5b);
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  candidate_5117a();
  candidate_5112e();
  if (*(char *)((int)unaff_A6 + 0xa1) != '\0') {
    *(char *)(unaff_A6 + 5) = *(char *)((int)unaff_A6 + 0xa1);
    *(undefined1 *)((int)unaff_A6 + 0xa1) = 0;
  }
  if (*(ushort *)((int)unaff_A6 + 0x8e) < 5) {
    candidate_51220();
  }
  candidate_511d2();
  if (*(ushort *)((int)unaff_A6 + 0x8e) < 5) {
    candidate_140c0();
  }
  uVar6 = *(ushort *)((int)unaff_A6 + 0x8e) < 6;
  if (*(ushort *)((int)unaff_A6 + 0x8e) == 6) {
    uVar4 = unaff_A6[0xd];
    uVar6 = 0x4fff < uVar4;
    unaff_A6[0xd] = uVar4 - 0x5000;
  }
  candidate_1ac38();
  candidate_427a();
  if (!(bool)uVar6) {
    if (*(short *)(unaff_A6 + 0xf) < -0x80) {
      candidate_13f1e();
      candidate_19dfc();
      candidate_12192();
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
    return;
  }
  candidate_9bb0();
  candidate_120f8();
  candidate_525ee();
  candidate_1da40();
  *(undefined1 *)((int)unaff_A6 + 0x9b) = 0xff;
  candidate_8cdb2();
  *unaff_A6 = candidate_51530;
  return;
}
