> 对AWS Cloud 亚马逊云的一些服务做一些概念性积累

---

# 概念补充

## AWS

——[AWS官网](https://aws.amazon.com/cn/?nc2=h_home&refid=7516b07a-6832-44d0-8623-e28575aa9ac3)

Amazon Web Services，即亚马逊提供的云计算服务平台，包含了广泛的云计算产品和服务，简称AWS。

---

## AWS Lambda

一项无服务器计算服务，它允许你运行代码而无需管理服务器。

只需要上传代码，AWS Lambda `会自动处理代码的执行、资源的配置和扩展`。

它可以帮助你减少基础设施管理的复杂性，并且按需自动扩展，按实际使用付费。

---

## Amazon Cognito

`身份和访问管理服务`，专门用于用户身份验证、授权以及用户资料管理。

帮助开发者更方便地构建安全的应用程序。

---

## API Gateway

一个托管的 API 服务。用于创建、发布、维护、监控和保护 API。

API Gateway 是 AWS 的一部分，通常用于 `AWS Lambda`、EC2、容器、其他服务或应用之间的通信。

---

## AWS CloudFormation

是一种用于自动化 AWS 基础设施部署的服务。

它使用户能够以代码的形式定义、管理和部署 AWS 资源。

CloudFormation 允许你通过模板（JSON 或 YAML 格式）来描述所需的 AWS 基础设施，并通过这些模板来`快速、可靠地创建和管理资源`。

---

## AWS IoT Core

亚马逊提供的一个云平台，旨在帮助开发者和企业通过互联网连接和管理大规模的物联网（IoT）设备。

它使得设备能够与云端应用程序进行安全、可靠的数据传输，并实现设备之间的通信。

AWS IoT Core 是一个 MQTT broker（消息中间件），它允许设备与云端的服务进行高效的通信。

AWS IoT Core 允许设备通过 MQTT、HTTP 或 WebSockets 等协议连接到云端，以进行双向通信。

因此，Lambda 函数和 IoT Core 之间的通信方式涉及使用 MQTT 协议，通常通过 IoT Core 的消息主题（topic） 进行交互。

---

## Alexa

Alexa 是由亚马逊推出的智能语音助手，广泛用于其 Echo 设备系列及其他第三方设备。

Alexa 通过语音识别技术允许用户与设备进行互动，执行各种任务。

> 配置技能

在alexa developer console中配置目标技能。

> 互动名称配置

使用 Voice Interaction Model 定义用户可以与 Alexa 互动的命令。

例如: Alexa，ask xx to yy. 的xx。

---

## Amazon S3

S3 (Simple Storage Service) 是亚马逊提供的一项 对象存储服务。

它提供了一个高度可扩展、可靠且低成本的方式来存储和检索任何量的数据。S3 允许用户通过互联网上传、下载和管理大量的数据文件（对象），例如文档、图片、视频、备份文件等。

# 相关工具

## AWS CDK

> Cloud Development Kit

一个开源软件开发框架，允许你使用熟悉的编程语言（如 TypeScript、JavaScript、Python、Java、C# 等）定义云资源。这意味着你可以通过代码来定义和管理你的 AWS 基础设施，而不是手动配置这些资源。

---

## AWS SAM

> Serverless Application Model

一种用于构建无服务器应用程序的开源框架。

它简化了 Lambda 函数、API Gateway、DynamoDB 表等 AWS 服务的配置和部署。

AWS SAM 是基于 AWS CloudFormation 的，并且专门为无服务器应用程序设计，能帮助你快速创建、部署和管理 AWS 上的无服务器应用。

---

## AWS CDK 和 AWS SAM 的区别

- AWS SAM： 专注于无服务器架构，简化了 Lambda 函数、API Gateway 等无服务器资源的定义和管理。通过简化的模板，专注于处理无服务器应用的构建与部署。

- AWS CDK： 提供了一个更为全面和灵活的开发框架，允许你用熟悉的编程语言（如 TypeScript、Python、Java、C# 等）编写 CloudFormation 模板，支持更复杂和定制化的云基础设施管理。它不仅适用于无服务器应用，还适用于多种云服务和资源的管理。

> AWS CDK 和 AWS SAM 既有重叠的功能，也各自具有独立的功能。

都是用来简化和自动化部署 AWS 资源的工具。

SAM支持本地调试，使用 sam local invoke 和 sam local start-api 来模拟 Lambda 函数和 API Gateway。

CDK 支持更多的AWS服务源，定义基础设施资源更加灵活

---