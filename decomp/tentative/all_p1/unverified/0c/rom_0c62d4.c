/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c62d4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c62d4(void)

{
  ushort uVar2;
  int iVar1;
  short sVar3;
  undefined4 extraout_A0;
  int *piVar4;
  undefined4 *unaff_A6;
  bool bVar5;
  undefined1 uVar6;
  undefined8 uVar7;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_4256();
  if (*(char *)((int)unaff_A6 + 0x96) == '\x02') {
    candidate_120f8();
  }
  candidate_1ad52();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  uVar7 = candidate_2e7c();
  unaff_A6[0xc] = (int)((longlong)uVar7 >> 0x21);
  unaff_A6[0xd] = (int)uVar7;
  uVar2 = candidate_29c6();
  *(undefined2 *)((int)unaff_A6 + 0x26) = *(undefined2 *)((short)(((uVar2 & 3) + 3) * 2) + 0xe7bb0);
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  *(undefined2 *)(unaff_A6 + 0x25) = 0xffff;
  *unaff_A6 = 0xc6382;
  unaff_A6[0xd] = unaff_A6[0xd] + -0x5000;
  candidate_1ac38();
  unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 4);
  candidate_427a();
  bVar5 = false;
  if (*(short *)(unaff_A6 + 0x25) != 0) {
    candidate_4c24();
    if (bVar5) {
      if ((*(char *)((int)unaff_A6 + 0x5a) == '\f') || (*(char *)((int)unaff_A6 + 0x5a) == '\r')) {
        candidate_4256();
        *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
        candidate_29c6();
        iVar1 = candidate_2e7c();
        unaff_A6[0xc] = iVar1 + unaff_A6[0xc];
        *(undefined2 *)(unaff_A6 + 0x25) = 0;
      }
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    if (7 < *(byte *)((int)unaff_A6 + 0x96)) {
      __m68k_trap(0xf);
    }
    piVar4 = *(int **)((short)((ushort)*(byte *)((int)unaff_A6 + 0x96) << 2) + 0xe7eda);
    do {
      if (*piVar4 == -1) break;
      piVar4 = piVar4 + 1;
      sVar3 = candidate_49e8();
    } while (sVar3 == 0);
  }
  uVar6 = false;
  if ((*(char *)((int)unaff_A6 + 0x61) == '\0') && (uVar7 = candidate_3c55c(), (bool)uVar6)) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    candidate_411c(uVar7,extraout_A0);
    uVar6 = 0;
    candidate_1ab98();
  }
  candidate_19504();
  if ((bool)uVar6) {
    *unaff_A6 = candidate_c66e8;
  }
  return;
}
