/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09c780.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9c780(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  ushort uVar5;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;
  char cVar7;

  *(undefined1 *)(unaff_A6 + 0x21) = 0xc0;
  bVar6 = false;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  candidate_1affc();
  if (bVar6) {
    candidate_1af9a();
  }
  *(undefined2 *)(unaff_A6 + 0xc) = 0;
  *(undefined2 *)(unaff_A6 + 0xd) = 0xff00;
  sVar3 = candidate_260c8();
  sVar4 = *(short *)(unaff_A6 + 0xc);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    sVar4 = -sVar4;
  }
  unaff_A6[0xc] = (int)sVar4 * (int)sVar3;
  unaff_A6[0xd] = (int)*(short *)(unaff_A6 + 0xd) * (int)sVar3;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x9c80c;
  func_0x000143e4();
  sVar4 = candidate_49e8();
  if (sVar4 == 0) {
    cVar7 = '\0';
    if (*(char *)((int)unaff_A6 + 0x85) == '\0') {
      candidate_3c5c6();
      *(char *)((int)unaff_A6 + 0x85) = -cVar7;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
      if (*(char *)((int)unaff_A6 + 0x85) != '\0') {
        candidate_4126();
        iVar2 = *(int *)(unaff_A5 + -0x1c24);
        do {
          unaff_A6 = (undefined4 *)unaff_A6[2];
          pcVar1 = (code *)*unaff_A6;
          *(undefined4 *)(iVar2 + -4) = 0x424a;
          (*pcVar1)();
        } while( true );
      }
      candidate_9f88();
      candidate_9ea8();
      *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
      *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
      if (-1 < (char)(*(char *)(unaff_A6 + 0x21) + -0x40)) {
        *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
      }
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0) {
        if ((*(byte *)((int)unaff_A6 + 0x5f) & 2) == 0) {
          uVar5 = candidate_29c6();
          *(ushort *)(unaff_A6 + 10) = ((uVar5 & 0xf) - 7) + *(short *)(unaff_A6 + 10);
          if (*(char *)((int)unaff_A6 + 0x6d) != '@') {
            uVar5 = candidate_29c6();
            *(ushort *)(unaff_A6 + 0xb) = ((uVar5 & 0xf) - 7) + *(short *)(unaff_A6 + 0xb);
          }
        }
        else {
          uVar5 = candidate_29c6();
          *(ushort *)(unaff_A6 + 10) = ((uVar5 & 0xf) - 7) + *(short *)(unaff_A6 + 10);
          *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 2;
        }
      }
      else {
        uVar5 = candidate_29c6();
        *(ushort *)(unaff_A6 + 0xb) = ((uVar5 & 0xf) - 7) + *(short *)(unaff_A6 + 0xb);
      }
      candidate_4256();
      *unaff_A6 = candidate_9c998;
      return;
    }
    candidate_427a();
    candidate_19504();
    if ((bool)cVar7) {
      *unaff_A6 = candidate_41ac;
    }
    return;
  }
  candidate_9f88();
  candidate_9ea8();
  uVar5 = candidate_29c6();
  *(ushort *)(unaff_A6 + 10) = ((uVar5 & 0xf) - 7) + *(short *)(unaff_A6 + 10);
  uVar5 = candidate_29c6();
  *(ushort *)(unaff_A6 + 0xb) = ((uVar5 & 0xf) - 7) + *(short *)(unaff_A6 + 0xb);
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
  candidate_4256();
  *unaff_A6 = candidate_9c998;
  return;
}
