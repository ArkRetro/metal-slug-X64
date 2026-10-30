/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d997c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d997c(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  undefined2 uVar4;
  short sVar5;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;

  uVar4 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x72) = uVar4;
  *unaff_A6 = 0xd9992;
  candidate_1abc8();
  *(short *)(unaff_A6 + 0x1c) = *(short *)(unaff_A6 + 0x1c) + 1;
  uVar3 = *(ushort *)(unaff_A6 + 0x1c) & 1;
  *(ushort *)(unaff_A6 + 0x1c) = uVar3;
  if (uVar3 == 0) {
    candidate_da350();
    candidate_da3d6();
    sVar5 = candidate_da272();
    *(short *)(unaff_A6 + 0x22) = sVar5 + *(short *)(unaff_A6 + 0x22);
    *(ushort *)(unaff_A6 + 0x22) = *(ushort *)(unaff_A6 + 0x22) & 0x1f;
  }
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x61) == -1) {
    if (*(char *)(unaff_A6 + 0x18) == -0x78) {
      *(char *)(unaff_A6[3] + 0x61) = *(char *)(unaff_A6[3] + 0x61) + '\x01';
    }
    *(undefined1 *)(unaff_A6 + 0x1f) = 0xff;
    *unaff_A6 = candidate_b0626;
    candidate_da40a();
    candidate_da23e();
  }
  bVar6 = *(char *)((int)unaff_A6 + 0x7d) != -1;
  if (*(char *)((int)unaff_A6 + 0x7d) == -1) {
    sVar5 = *(short *)((int)unaff_A6 + 0x72);
    bVar6 = sVar5 == 0;
    sVar5 = sVar5 + -1;
    *(short *)((int)unaff_A6 + 0x72) = sVar5;
    if (sVar5 == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x7d) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x7b) = 0xff;
      *unaff_A6 = candidate_d9a3e;
      bVar6 = false;
    }
  }
  candidate_da21e();
  if (!bVar6) {
    candidate_da370();
    if (!bVar6) {
      return;
    }
  }
  *(undefined1 *)(unaff_A6 + 0x1f) = 0xff;
  candidate_da23e();
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
