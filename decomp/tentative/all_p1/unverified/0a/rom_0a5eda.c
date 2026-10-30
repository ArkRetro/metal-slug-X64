/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a5eda.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a5eda(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  byte bVar4;
  undefined1 uVar5;
  char cVar6;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar7;

  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x1e;
  candidate_a6bb2();
  uVar5 = candidate_260c8();
  *(undefined1 *)(unaff_A6 + 0x20) = uVar5;
  cVar6 = candidate_a6a32();
  bVar4 = *(byte *)(unaff_A6 + 0x20) - cVar6;
  *(byte *)(unaff_A6 + 0x20) = bVar4;
  if ((short)((ushort)bVar4 << 8) < 0) {
    *(undefined1 *)(unaff_A6 + 0x20) = 0;
  }
  candidate_a6bb2();
  uVar5 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x82) = uVar5;
  candidate_a6bb2();
  uVar5 = candidate_260c8();
  *(undefined1 *)(unaff_A6 + 0x21) = uVar5;
  candidate_a6bb2();
  uVar5 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x83) = uVar5;
  *(undefined1 *)((int)unaff_A6 + 0x81) = 0;
  *unaff_A6 = 0xa5f6a;
  candidate_1abc8();
  candidate_427a();
  sVar3 = *(short *)((int)unaff_A6 + 0x7e);
  *(short *)((int)unaff_A6 + 0x7e) = sVar3 + -1;
  if (sVar3 == 0) {
    *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x1e;
    bVar7 = false;
    candidate_a6950();
    if (bVar7) {
      *unaff_A6 = candidate_a5fb0;
    }
  }
  candidate_1abc8();
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0x5f) & 0x80) != 0) {
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
