/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07643a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7643a(void)

{
  code *pcVar1;
  int iVar2;
  short *psVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined8 uVar5;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1110;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1110;
  candidate_9ea8();
  psVar3 = (short *)((uint)*(ushort *)((int)unaff_A6 + 0x7e) * 6 +
                    (uint)*(ushort *)(unaff_A6 + 0x1f) * 0x24 + 0xe3ba4);
  *(undefined1 *)((int)unaff_A6 + 0x12) = *(undefined1 *)(psVar3 + 2);
  *(undefined1 *)((int)unaff_A6 + 0x13) = *(undefined1 *)(psVar3 + 2);
  *(short *)(unaff_A6 + 10) = *psVar3 + 0x120;
  *(short *)(unaff_A6 + 0xb) = psVar3[1] + 0x1c0;
  if (*(short *)((int)unaff_A6 + 0x7e) == 0) {
    candidate_120f8();
  }
  candidate_1ad52();
  uVar5 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar5;
  candidate_4256();
  candidate_40bc();
  *unaff_A6 = 0x764e6;
  candidate_1ac38();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0xf) < 0xe0) {
    *unaff_A6 = candidate_76506;
  }
  bVar4 = false;
  candidate_1945c();
  if (bVar4) {
    *(char *)(unaff_A6[3] + 0x60) = *(char *)(unaff_A6[3] + 0x60) + '\x01';
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
