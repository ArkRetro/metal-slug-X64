/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09db44.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9db44(void)

{
  char cVar1;
  undefined4 uVar2;
  short sVar4;
  int iVar3;
  undefined2 uVar5;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar6;
  bool bVar7;

  candidate_9f432();
  sVar4 = candidate_260c8();
  iVar3 = sVar4 * 0x100;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar3 = sVar4 * -0x100;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar3;
  uVar6 = 0;
  candidate_1436a();
  candidate_9f0fa();
  if ((bool)uVar6) {
    candidate_9f314();
    if ((bool)uVar6) {
      if ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) != 0) {
        if (*(char *)((int)unaff_A6 + 0x95) == '\0') {
code_r0x0009e040:
          *(undefined2 *)(unaff_A6 + 0x22) = 0x7fff;
          *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xdf;
          *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xbf;
          *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) | 0x10;
          *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x200;
          *(undefined1 *)((int)unaff_A6 + 0x93) = 6;
          candidate_4256();
          *unaff_A6 = 0x9e026;
          uVar6 = 0;
          candidate_1436a();
          candidate_427a();
          if ((bool)uVar6) {
            *unaff_A6 = candidate_9d9c0;
          }
          goto LAB_0009e7ac;
        }
        *(char *)((int)unaff_A6 + 0x95) = *(char *)((int)unaff_A6 + 0x95) + -1;
      }
      candidate_9f412();
      uVar5 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x22) = uVar5;
      if ((*(byte *)((int)unaff_A6 + 0x97) & 2) == 0) {
        candidate_4256();
      }
      else {
        candidate_4256();
      }
      *unaff_A6 = 0x9e194;
      uVar6 = 0;
      candidate_1436a();
      candidate_427a();
      if ((bool)uVar6) {
        *unaff_A6 = candidate_9e1ae;
      }
    }
    else {
      candidate_9f348();
      if ((bool)uVar6) {
        if ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) != 0) {
          if (*(char *)((int)unaff_A6 + 0x95) == '\0') goto code_r0x0009e040;
          *(char *)((int)unaff_A6 + 0x95) = *(char *)((int)unaff_A6 + 0x95) + -1;
        }
        candidate_9f412();
        candidate_9f3b0();
        if ((*(byte *)((int)unaff_A6 + 0x97) & 2) == 0) {
          candidate_4256();
        }
        else {
          candidate_4256();
        }
        *unaff_A6 = 0x9e0b2;
        uVar6 = 0;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
          candidate_143dc();
        }
        else {
          candidate_1436a();
        }
        candidate_427a();
        if ((bool)uVar6) {
          candidate_9f432();
          uVar5 = candidate_260c8();
          *(undefined2 *)(unaff_A6 + 0x22) = uVar5;
          candidate_1b04a();
          *(undefined4 *)((int)unaff_A6 + 0x6e) = extraout_A0;
          *unaff_A6 = candidate_9d9c0;
          if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
            *unaff_A6 = candidate_9e34c;
          }
        }
        goto LAB_0009e7ac;
      }
      if ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) == 0) {
        *(byte *)((int)unaff_A6 + 0x97) = *(byte *)((int)unaff_A6 + 0x97) | 2;
        candidate_9f432();
        uVar2 = 0x8000;
        if ((*(byte *)((int)unaff_A6 + 0x97) & 1) == 0) {
          uVar2 = 0xffff8000;
        }
        *(undefined4 *)((int)unaff_A6 + 0x62) = uVar2;
        *(undefined1 *)((int)unaff_A6 + 0x92) = 0xf;
        candidate_4256();
        *unaff_A6 = candidate_9d964;
        uVar6 = 0;
        candidate_1436a();
        candidate_9f0fa();
        if ((bool)uVar6) {
          candidate_9f196();
          if ((bool)uVar6) {
            *unaff_A6 = candidate_9e25a;
          }
          else {
            *(byte *)((int)unaff_A6 + 0x97) = *(byte *)((int)unaff_A6 + 0x97) ^ 1;
            *(undefined1 *)((int)unaff_A6 + 0x92) = 0xf;
            *(int *)((int)unaff_A6 + 0x62) = -*(int *)((int)unaff_A6 + 0x62);
          }
        }
        candidate_9efc8();
        *(char *)((int)unaff_A6 + 0x92) = *(char *)((int)unaff_A6 + 0x92) + -1;
        uVar6 = false;
        if (*(char *)((int)unaff_A6 + 0x92) < '\x01') {
          *(byte *)((int)unaff_A6 + 0x97) = *(byte *)((int)unaff_A6 + 0x97) ^ 1;
          *(undefined1 *)((int)unaff_A6 + 0x92) = 0xf;
          iVar3 = -*(int *)((int)unaff_A6 + 0x62);
          *(int *)((int)unaff_A6 + 0x62) = iVar3;
          uVar6 = iVar3 != 0;
        }
        candidate_427a();
      }
      else {
        *(byte *)((int)unaff_A6 + 0x97) = *(byte *)((int)unaff_A6 + 0x97) & 0xfd;
        candidate_9f432();
        sVar4 = candidate_260c8();
        iVar3 = sVar4 * 0x100;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
          iVar3 = sVar4 * -0x100;
        }
        *(int *)((int)unaff_A6 + 0x62) = iVar3;
        candidate_4256();
        *unaff_A6 = 0x9da2e;
        uVar6 = 0;
        candidate_1436a();
        candidate_427a();
      }
      candidate_1af9a();
      if (!(bool)uVar6) {
        candidate_9f0bc();
        if ((bool)uVar6) {
          *unaff_A6 = candidate_9e00a;
          uVar6 = 0;
        }
        else {
          candidate_9f0fa();
          if ((bool)uVar6) {
            candidate_9f196();
            if ((bool)uVar6) {
              *unaff_A6 = candidate_9e25a;
              uVar6 = 0;
            }
          }
          else {
            candidate_9f2bc();
            if ((bool)uVar6) {
              *unaff_A6 = candidate_9dd12;
              uVar6 = 0;
            }
            candidate_9f156();
            if ((bool)uVar6) {
              *unaff_A6 = candidate_9e250;
            }
            uVar6 = false;
            if (*(char *)(unaff_A6 + 0x24) < '\0') {
              candidate_9f250();
              if ((bool)uVar6) {
                *unaff_A6 = candidate_9db44;
                uVar6 = 0;
              }
            }
            else {
              cVar1 = *(char *)(unaff_A6 + 0x24);
              uVar6 = cVar1 == '\0';
              *(char *)(unaff_A6 + 0x24) = cVar1 + -1;
            }
            candidate_9f280();
            if ((bool)uVar6) {
              *unaff_A6 = candidate_9ddce;
              uVar6 = 0;
              if ((*(byte *)(unaff_A6 + 0x26) & 0x40) != 0) {
                *(byte *)(unaff_A6 + 0x26) = *(byte *)(unaff_A6 + 0x26) | 0x80;
                candidate_9f432();
                *unaff_A6 = candidate_9db44;
                uVar6 = 0;
              }
            }
          }
        }
        candidate_9f348();
        if ((bool)uVar6) {
          *unaff_A6 = candidate_9e062;
          uVar6 = 0;
        }
        candidate_9f314();
        if ((bool)uVar6) {
          *unaff_A6 = candidate_9e122;
          uVar6 = 0;
        }
      }
      candidate_9f208();
      if ((bool)uVar6) {
        *unaff_A6 = candidate_9e040;
        uVar6 = 0;
      }
      candidate_9f1c8();
      if ((bool)uVar6) {
        *unaff_A6 = candidate_9dc5c;
      }
    }
  }
  else {
    if ((*(byte *)((int)unaff_A6 + 0x97) & 2) == 0) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *unaff_A6 = 0x9dbbe;
    uVar6 = 0;
    candidate_9efc8();
    candidate_427a();
    candidate_9f4ac();
    candidate_9f23a();
    if ((bool)uVar6) {
      *unaff_A6 = candidate_9dfb2;
      uVar6 = 0;
    }
    candidate_1af9a();
    if ((bool)uVar6) {
      *unaff_A6 = candidate_9dfb2;
    }
    else {
      candidate_9f314();
      if ((bool)uVar6) {
        *unaff_A6 = candidate_9e122;
        uVar6 = 0;
      }
      candidate_9f348();
      if ((bool)uVar6) {
        *unaff_A6 = candidate_9dfb2;
        uVar6 = 0;
      }
      candidate_9f0bc();
      if ((bool)uVar6) {
        *unaff_A6 = candidate_9dfb2;
      }
      else {
        candidate_9f0fa();
        if ((bool)uVar6) {
          *unaff_A6 = candidate_9dfb2;
          bVar7 = false;
          candidate_9f196();
          if (bVar7) {
            *unaff_A6 = candidate_9e25a;
          }
        }
        else {
          candidate_9f156();
          if ((bool)uVar6) {
            *unaff_A6 = candidate_9e250;
            uVar6 = 0;
          }
          candidate_9f250();
          if (!(bool)uVar6) {
            *unaff_A6 = candidate_9dfb2;
          }
        }
      }
    }
  }
  if (((*(byte *)(unaff_A6 + 0x26) & 4) != 0) &&
     (*(short *)((int)unaff_A6 + 0x8a) = *(short *)((int)unaff_A6 + 0x8a) + -1,
     *(short *)((int)unaff_A6 + 0x8a) < 1)) {
    *unaff_A6 = candidate_9de84;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_9e34c;
  }
LAB_0009e7ac:
  sVar4 = *(short *)(unaff_A6 + 0x22);
  bVar7 = sVar4 == 0;
  *(short *)(unaff_A6 + 0x22) = sVar4 + -1;
  candidate_4c24();
  if (bVar7) {
    candidate_9f53e();
    candidate_9f5fa();
    cVar1 = *(char *)((int)unaff_A6 + 0x5a);
    if (cVar1 == '\x02') {
      *unaff_A6 = candidate_9e8c2;
    }
    if (cVar1 == '\x01') {
      *unaff_A6 = candidate_9e920;
    }
    if (cVar1 == '\f') {
      *unaff_A6 = candidate_9e920;
    }
    if (cVar1 == '\r') {
      *unaff_A6 = candidate_9e920;
    }
    if (cVar1 == '\t') {
      *unaff_A6 = candidate_9e920;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_9eab8;
    }
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_9ec34;
  }
  bVar7 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar7) {
    *unaff_A6 = candidate_9ec34;
  }
  return;
}
