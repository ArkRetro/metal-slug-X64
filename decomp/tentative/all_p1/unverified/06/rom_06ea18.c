/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06ea18.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6ea18(void)

{
  code *pcVar1;
  int iVar2;
  undefined2 uVar3;
  byte *pbVar4;
  char cVar5;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;

  *(undefined2 *)(unaff_A6 + 0x1c) = 0xff;
  *(undefined1 *)((int)unaff_A6 + 0x7b) = 0xff;
  unaff_A6[0x14] = 0xe2238;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0;
  *unaff_A6 = 0x6ea38;
  bVar6 = false;
  candidate_409a();
  if (!bVar6) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  cVar5 = candidate_aab84();
  if (cVar5 < '\0') {
    *unaff_A6 = 0x6e9f4;
    return;
  }
  candidate_1aca2();
  uVar3 = *(undefined2 *)(extraout_A0 + 0x78);
  *(undefined2 *)(unaff_A6 + 10) = *(undefined2 *)(extraout_A0 + 0x76);
  *(undefined2 *)(unaff_A6 + 0xb) = uVar3;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(extraout_A0 + 0x1e);
  *(undefined2 *)((int)unaff_A6 + 0x6e) = *(undefined2 *)(extraout_A0 + 0x6e);
  if (*(short *)(extraout_A0 + 0x70) != *(short *)(unaff_A6 + 0x1c)) {
    *(short *)(unaff_A6 + 0x1c) = *(short *)(extraout_A0 + 0x70);
    candidate_9f88();
    candidate_9ea8();
    *(undefined1 *)((int)unaff_A6 + 0x7b) = 0xff;
  }
  *(undefined1 *)(unaff_A6 + 0x1f) = *(undefined1 *)(unaff_A6[3] + 0x7c);
  cVar5 = *(char *)(unaff_A6[3] + 0x7b);
  if (cVar5 != *(char *)((int)unaff_A6 + 0x7b)) {
    *(char *)((int)unaff_A6 + 0x7b) = cVar5;
    candidate_4256();
  }
  candidate_1abc8();
  candidate_427a();
  candidate_aa9e4();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    iVar2 = unaff_A6[3];
    pbVar4 = (byte *)(iVar2 + 0x80);
    *pbVar4 = *pbVar4 | 1;
    *(undefined1 *)(iVar2 + 0x5a) = *(undefined1 *)((int)unaff_A6 + 0x5a);
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  return;
}
