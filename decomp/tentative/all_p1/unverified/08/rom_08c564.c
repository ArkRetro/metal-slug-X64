/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08c564.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8c564(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  char cVar4;
  short *psVar5;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;
  undefined1 uVar7;

  sVar3 = *(short *)(unaff_A6 + 0x1c) + -1;
  *(short *)(unaff_A6 + 0x1c) = sVar3;
  if (sVar3 != 0) {
    sVar3 = *(short *)((int)unaff_A6 + 0x86);
    *(short *)((int)unaff_A6 + 0x86) = sVar3 + -1;
    if (sVar3 == 0) {
      *(undefined1 *)(unaff_A6 + 0x1f) = 0xff;
    }
    (**(code **)(*(int *)((int)unaff_A6 + 0x82) + 0x14))();
    bVar6 = false;
    if (*(char *)(unaff_A6 + 0x20) == '\0') {
      if (*(short *)(unaff_A6 + 0x1e) == 0) {
        cVar4 = candidate_8c0cc();
        bVar6 = false;
        if (-1 < cVar4) {
          (**(code **)(*(int *)((int)unaff_A6 + 0x82) + 0x10))();
          candidate_8cbf8();
          if (*(char *)(*(int *)((int)unaff_A6 + 0x82) + 2) != '\0') {
            *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
            candidate_411c();
            candidate_1acf2();
          }
          *(undefined1 *)(unaff_A6 + 0x23) = 2;
          *unaff_A6 = 0x8c66a;
          (**(code **)(*(int *)((int)unaff_A6 + 0x82) + 0x14))();
          candidate_427a();
          cVar4 = *(char *)(unaff_A6 + 0x23) + -1;
          *(char *)(unaff_A6 + 0x23) = cVar4;
          if (cVar4 != '\0') {
            return;
          }
          goto LAB_0008c684;
        }
      }
      else {
        sVar3 = *(short *)(unaff_A6 + 0x1e);
        bVar6 = sVar3 == 0;
        *(short *)(unaff_A6 + 0x1e) = sVar3 + -1;
      }
    }
    candidate_427a();
    if (bVar6) {
      if (*(char *)((int)unaff_A6 + 0x8a) != '\0') {
        psVar5 = (short *)0xe553e;
        do {
          sVar3 = *psVar5;
          if (sVar3 < 0) goto LAB_0008c602;
          psVar5 = psVar5 + 1;
        } while (*(short *)((int)unaff_A6 + 0x6e) != sVar3);
        goto LAB_0008c684;
      }
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x8a) = 0xff;
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    }
LAB_0008c602:
    uVar7 = false;
    if (((*(char *)((int)unaff_A6 + 0x8b) != '\0') && (candidate_409a(), (bool)uVar7)) ||
       (candidate_c8bec(), !(bool)uVar7)) {
      return;
    }
  }
LAB_0008c684:
  candidate_8cbf8();
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
