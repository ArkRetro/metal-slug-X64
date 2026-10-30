/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0855ea.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_855ea(void)

{
  short sVar1;
  uint uVar2;
  undefined2 uVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;
  undefined8 uVar6;

  do {
    uVar4 = false;
    if (*(short *)(unaff_A6 + 0x1d) != 0) {
      sVar1 = *(short *)(unaff_A6 + 0x1d);
      uVar4 = sVar1 == 0;
      *(short *)(unaff_A6 + 0x1d) = sVar1 + -1;
LAB_00085602:
      candidate_85e52();
      if ((bool)uVar4) {
LAB_000856ca:
        candidate_1ad52();
        uVar6 = candidate_3b332();
        unaff_A6[0xc] = (int)((ulonglong)uVar6 >> 0x20) + unaff_A6[0xc];
        unaff_A6[0xd] = (int)uVar6 + unaff_A6[0xd];
        *unaff_A6 = 0x85704;
        uVar2 = unaff_A6[0xd];
        uVar4 = 0xffff9fff < uVar2;
        unaff_A6[0xd] = uVar2 + 0x6000;
        candidate_1ac38();
        candidate_427a();
      }
      else {
        candidate_85e60();
        candidate_85d2e();
        candidate_427a();
      }
LAB_0008568c:
      candidate_4c24();
      if ((bool)uVar4) {
        *(byte *)((int)unaff_A6 + 0x87) = *(byte *)((int)unaff_A6 + 0x87) | 6;
        if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
          *unaff_A6 = 0x85880;
        }
        *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
        uVar4 = 0;
      }
      candidate_1945c();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_858c4;
      }
      if (*(char *)((int)unaff_A6 + 0x87) != '\0') {
        *(char *)((int)unaff_A6 + 0x87) = *(char *)((int)unaff_A6 + 0x87) + -1;
      }
      return;
    }
    candidate_85dce();
    if (!(bool)uVar4) goto LAB_00085602;
    if ((((0x10 < *(short *)(unaff_A6 + 0xf)) && (*(short *)(unaff_A6 + 0xf) < 0x130)) &&
        (0x110 < *(short *)(unaff_A6 + 0x10))) &&
       ((bVar5 = *(ushort *)(unaff_A6 + 0x10) < 0x1f0, (short)*(ushort *)(unaff_A6 + 0x10) < 0x1f0
        && (candidate_85e60(), bVar5)))) {
      candidate_854bc();
      *(char *)(unaff_A6 + 0x18) = -!bVar5;
      if (bVar5) {
        if (0 < *(short *)((int)unaff_A6 + 0x7e)) {
          *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
        }
        candidate_1b1dc();
        *unaff_A6 = 0x85670;
        uVar4 = 0;
        if (*(char *)(unaff_A6 + 0x18) == '\0') {
          candidate_1436a();
          candidate_427a();
          goto LAB_0008568c;
        }
      }
    }
    if (*(short *)((int)unaff_A6 + 0x7e) == 0) goto LAB_000856ca;
    uVar3 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x1d) = uVar3;
    *unaff_A6 = candidate_855ea;
  } while( true );
}
